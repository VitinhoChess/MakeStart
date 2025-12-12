#include "missile.hpp"
#include <cmath>
#include <iostream>



// constructeur

missile::missile(const sf::Texture& texture)
: m_tex(&texture), m_sprite(texture)
	{
		sf::FloatRect b = m_sprite.getLocalBounds();
		m_sprite.setOrigin(b.width/2.f, b.height/2.f); // centrer l'origine
		m_sprite.setScale(0.1f, 0.1f); // taille du sprite
	}

void missile::applyTextureKeepCenter(const sf::Texture& tex) 
	{
		sf::Vector2f pos = m_sprite.getPosition();
		sf::Vector2f scl = m_sprite.getScale();
		m_sprite.setTexture(tex, true);
		sf::FloatRect b = m_sprite.getLocalBounds();
		m_sprite.setOrigin(b.width/2.f, b.height/2.f);
		m_sprite.setScale(scl);
		m_sprite.setPosition(pos);
	}

void missile::launch(sf::Vector2f startPos, float speed, float angleDeg) 
	{
		// declare le missile comme alive, lui calcule sa vitesse x et y, initialise sa pos et sa rotation
		const float angle = deg2grad(angleDeg);

		// calcul vitesse (maths)
		m_vel.x = std::cos(angle) * speed;
		m_vel.y = std::sin(angle) * speed;

		m_sprite.setPosition(startPos);
		m_sprite.setRotation(angleDeg); // gerer l'orientation du sprite ici

		m_exploding = false;
		m_explIndex = 0;
		m_explTimer = 0.f;

		m_alive = true;
		m_hasHit = false;
		m_hasHitGround = false;
		m_groundCounted = false;
	}

void missile::triggerExplosion() 
	{
		
		if(!m_alive || m_exploding)
			return; // deja mort ou en explosion
		
		m_exploding = true;
		m_explIndex = 0; // remettre les coefficients à zero
		m_explTimer = 0.f;
		m_vel = {0.f, 0.f}; // arreter la vitesse

		// SON
		if(!m_explBuffer.loadFromFile("graphismes/explosion8bits.wav"))
			std::cerr << "erreur load explosion8bits.wav" << std::endl;
		m_explSound.setBuffer(m_explBuffer);
		m_explSound.setVolume(100.f); // volume son 0 à 100
		m_explSound.setPitch(1.f); // aigu (fréquence et durée) 0.5 grave, 2.0 aigu
		m_explSound.play();

		// visuel
		m_sprite.setRotation(0);
		applyTextureKeepCenter(*m_expl[0]);
		m_sprite.setScale(0.5f, 0.5f); // set scale des explosions ici


	}

void missile::update(float dt, const sf::RenderWindow& window) 
	{
		if(!m_alive)
			return;

		if(m_exploding) // animation 3 frames
			{
				m_explTimer += dt;
				if(m_explTimer >= m_explFrameDt) 
					{
						m_explTimer = 0.f;
						++m_explIndex;

						if(m_explIndex < 3 && m_expl[m_explIndex]) // double sécurité ? 
							{
								applyTextureKeepCenter(*m_expl[m_explIndex]);
							}
						else 
							{
								m_alive = false;
							}
					}
				return;
			}

		// bouger
		m_sprite.move(m_vel *dt);

		// rebonds murs g&d
		// calcul des vmin et vmax du sprite
		const float halfW = m_sprite.getGlobalBounds().width * 0.5f; 
		const float minX = halfW;
		const float maxX = static_cast<float>(window.getSize().x) - halfW; // conversion int to float

		// inversion vitesse en x si bords dépassés
		sf::Vector2f pos = m_sprite.getPosition();
		if(pos.x < minX) {pos.x = minX; m_vel.x = -m_vel.x; m_sprite.setPosition(pos);}
		if(pos.x > maxX) {pos.x = maxX; m_vel.x = -m_vel.x; m_sprite.setPosition(pos);}

		// ajuster l'angle du sprite. Pas obligé si vitesse et trajectoire constante
		if(std::abs(m_vel.x) + std::abs(m_vel.y) > 0.001f) 
			{
				float a = std::atan2(m_vel.y, m_vel.x) * 180.f / 3.1415926535f;
				m_sprite.setRotation(a);
			}
		
		// coord du sol
		const float halfH = m_sprite.getGlobalBounds().height * 0.5f;
		const float maxY = static_cast<float>(window.getSize().y) - halfH;

		// déclenche le missile si sol est atteint (maxY) 
		if(m_sprite.getPosition().y >= maxY && !m_hasHitGround) 
			{
				m_hasHitGround = true;
				triggerExplosion();
			}

// ----- fin upadte() -----		
	}

sf::FloatRect missile::getHitbox() const 
	{
		sf::Vector2f pos = m_sprite.getPosition();
		float left = pos.x - m_hitboxSize.x / 2.f + m_hitboxOffset.x;
		float top = pos.y - m_hitboxSize.y / 2.f + m_hitboxOffset.y;

		return {left, top, m_hitboxSize.x, m_hitboxSize.y};
	}

void missile::drawHitbox(sf::RenderWindow& window, sf::Color color) const 
	{
		if(!m_alive)
			return;
		sf::FloatRect hb = getHitbox();
		sf::RectangleShape r;
		r.setPosition(hb.left, hb.top);
		r.setSize({hb.width, hb.height});
		r.setFillColor(sf::Color::Transparent);
		r.setOutlineColor(color);
		r.setOutlineThickness(1.f);
		window.draw(r);
	}

void missile::draw(sf::RenderWindow& window) const 
	{
		if(m_alive)
			window.draw(m_sprite);
	}

bool missile::intersects(const sf::FloatRect& other) const 
	{
		return m_alive && m_sprite.getGlobalBounds().intersects(other);
	}

void missile::invertAlive() 
	{
		m_alive = !m_alive;
	}




















		
		
