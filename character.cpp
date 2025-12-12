#include "character.hpp"
#include <iostream>



// CONSTRUCTEUR -- INITIALISE TRALALERO
character::character(const std::string& texturePath, const std::string& texturePathWalk, float startX, float startY) 
      {
// VISUEL - charger les textures
            if(!m_texture.loadFromFile(texturePath)) 
                  std::cerr << "erreur load from file idle" << texturePath << std::endl;
		if(!m_textureW.loadFromFile(texturePathWalk)) 
                  std::cerr << "erreur load from file walk" << texturePathWalk << std::endl;
		
            // besoin de set ici sinon pb avec setOrigin
            m_sprite.setTexture(m_texture);

		// mettre l'axe de pivotement au centre
		sf::FloatRect bounds = m_sprite.getLocalBounds(); // bounds = boite invisible
		m_sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
		m_sprite.setScale(-m_size, m_size); // note : '-' pour orientation de départ vers la droite
		
            m_sprite.setPosition(startX, startY);
// FIN VISUEL
		
// AUDIO
		if(!m_jumpBuffer.loadFromFile("graphismes/jump8bits.wav"))
			std::cerr << "erreur load jump8bits.wav" << std::endl;
		m_jumpSound.setBuffer(m_jumpBuffer);
		m_jumpSound.setVolume(100.f); // volume son 0 à 100
		m_jumpSound.setPitch(1.f); // aigu (fréquence et durée) 0.5 grave, 2.0 aigu
		
// FIN AUDIO
      }

void character::update(float dt) 
      {
// ----- animation saut -----
		if(m_jumpTimer > 0.f)
			{
				m_sprite.setTexture(m_textureW);
				m_jumpTimer -= dt; 
				if(m_jumpTimer <= 0.f) 
					{
						m_jumpTimer = 0.f;
					}
			}
// ----- fin animation saut -----
// ----- animation marche -----
		else if(m_justStartedMoving) 
			{
				m_sprite.setTexture(m_textureW);
				m_animTimer = 0.f;
				m_useAltFrame = false;
			} 
		else if(m_isMoving)
			{
				m_animTimer += dt;
				if(m_animTimer >= 0.20f) // temps de basculement entre 2 sprites
					{
						m_animTimer = 0.f;
						m_useAltFrame = !m_useAltFrame;
						if(m_useAltFrame) 
							{
								m_sprite.setTexture(m_textureW);
							}
						else 
							{
								m_sprite.setTexture(m_texture);
							}
					}
			}
		else 
			{
				m_sprite.setTexture(m_texture);
				m_animTimer = 0.f;
				m_useAltFrame = false;
			}
// ----- fin animation marche -----

// ----- physique saut -----
			m_velocityY += m_gravity * dt;
			m_sprite.move(0.f, m_velocityY * dt);
// ----- fin physique saut -----
	}

void character::move(float dx, float dy) 
      {
            m_sprite.move(dx, dy);
      }

void character::setPositionMan(float x, float y, int orientation) 
	{
		m_sprite.setScale((orientation) * m_size, m_size);
		m_sprite.setPosition(x, y);
	}

void character::handleInput(float dt)
	{
		float speed = 200.f; // pixels/sec
		bool movingThisFrame = false;
		
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) 
			{	
                  	move(-speed * dt, 0.f);
				m_sprite.setScale(m_size, m_size);
				movingThisFrame = true;
			}
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
                  	move(speed * dt, 0.f);
				m_sprite.setScale(-m_size, m_size);
				movingThisFrame = true;
			}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && m_isOnGround) 
			{
				m_velocityY = -400.f;
				m_isOnGround= false;
				m_jumpTimer = 0.30f;
				// audio
				m_jumpSound.play();
			}
		if(movingThisFrame && !m_isMoving) 
			{
				m_justStartedMoving = true;
			}
		else 
			{
				m_justStartedMoving = false;
			}
		
		// mémoriser l'état de la frame i pour la frame i+1
		m_isMoving = movingThisFrame;
	}

void character::clampToWindow(const sf::RenderWindow& window) 
	{
		sf::FloatRect gb = m_sprite.getGlobalBounds();
		float halfW = gb.width / 2.0f;
		float halfH = gb.height / 2.0f;

		float minX = halfW;
		float maxX = window.getSize().x - halfW;
		//float minY = halfH; pas besoin de min Y
		float maxY = window.getSize().y - halfH;

		// variable de la position actuelle du sprite
		sf::Vector2f pos = m_sprite.getPosition();

		// clamps
		if(pos.x < minX)
			pos.x = minX;
		if(pos.x > maxX)
			pos.x = maxX;

		if(pos.y > maxY) 
			{
				pos.y = maxY;
				m_velocityY = 0.f; // touche le sol -> stop
				m_isOnGround = true;
			}
		
		m_sprite.setPosition(pos);
		
	}

sf::FloatRect character::getHitbox() const 
	{
		sf::Vector2f pos = m_sprite.getPosition();
		// centrer origine
		float left = pos.x - m_hitboxSize.x / 2.f + m_hitboxOffset.x; 
		float top = pos.y - m_hitboxSize.y/ 2.f + m_hitboxOffset.y;

		return {left, top, m_hitboxSize.x, m_hitboxSize.y};
	}
void character::drawHitbox(sf::RenderWindow& window, sf::Color color) const 
	{
		sf::FloatRect hb = getHitbox();
		sf::RectangleShape r;
		r.setPosition(hb.left, hb.top);
		r.setSize({hb.width, hb.height});
		r.setFillColor(sf::Color::Transparent);
		r.setOutlineColor(color);
		r.setOutlineThickness(1.f);
		window.draw(r);
	}


void character::draw(sf::RenderWindow& window) 
      {
            window.draw(m_sprite);
      }
sf::FloatRect character::getBounds() const 
      {
            return m_sprite.getGlobalBounds();
      }

void character::takeDammage(float amount) 
	{
		m_pv -= amount;
		if(m_pv < 0.f)
			m_pv = 0.f;
	}

void character::setupHealthBar(const sf::Vector2f& pos, const sf::Vector2f& size) 
	{
		m_pvBarPos = pos;
		m_pvBarSize = size;
	}


void character::drawHealthBar(sf::RenderWindow& window) const
	{
		// fond
		sf::RectangleShape back;
		back.setPosition(m_pvBarPos);
		back.setSize(m_pvBarSize);
		back.setFillColor(m_pvBackColor);

		float ratio = getHealthRatio();
		if(ratio > 1.f) ratio = 1.f;
		if(ratio < 0.f) ratio = 0.f;

		// bdv
		sf::RectangleShape front;
		front.setPosition(m_pvBarPos);
		front.setSize(sf::Vector2f(m_pvBarSize.x * ratio, m_pvBarSize.y));
		front.setFillColor(m_pvFrontColor);

		window.draw(back);
		window.draw(front);
		
	}
