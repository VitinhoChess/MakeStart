#include "crocodilo.hpp"
#include <iostream>
#include <algorithm>
#include <cmath>

// constructeur : initialiser crocodilo
crocodilo::crocodilo(const std::string& texturePath, float y, float leftX, float rightX, float speed)
	{
		m_leftX = leftX;
		m_rightX = rightX;
		m_speed = speed;
		m_altitude = y;

		 if(!m_textureCro.loadFromFile(texturePath)) 
                  std::cerr << "erreur load from file crocodilo" << texturePath << std::endl;
		m_spriteCro.setTexture(m_textureCro);

		// centrer l'origine pour les flips
		const sf::FloatRect b = m_spriteCro.getLocalBounds();
		m_spriteCro.setOrigin(b.width/2.f, b.height/2.f);

		// taille et position initiale
		m_spriteCro.setPosition(m_leftX, m_altitude);
		m_spriteCro.setScale(m_size, m_size);
	}

void crocodilo::update(float dt) 
	{

		m_time += dt;
		
		// deplacement horizontal
		sf::Vector2f pos = m_spriteCro.getPosition();
		pos.x += m_dir * m_speed * dt;

		// rebonds dans la zone de patrouille
		if(pos.x <= m_leftX){pos.x = m_leftX; m_dir = 1;}
		if(pos.x >= m_rightX){pos.x = m_rightX; m_dir = -1;}

		// oscillations et turbulences
		const float deuxPi = 6.28318530718f;
		float osc = m_oscAmp * std::sin(deuxPi * m_turbFreq * m_time + 1.234f);

		float turb = m_turbAmp
			* std::sin(deuxPi * m_turbFreq * m_time + 1.234f)
			*std::sin(deuxPi * m_turbFreq * 0.37f * m_time + 4.321f);
		float yFinal = m_altitude + osc + turb; // altitude finale apres perturbations

		// flip visuel selon direction
		m_spriteCro.setScale(m_dir > 0 ? -m_size : m_size, m_size);

		// update la position en x et y
		m_spriteCro.setPosition(pos.x, yFinal);
	}

void crocodilo::clampToWindow(const sf::RenderWindow& window) 
	{
		// empecher toute sortie de l'écran
		const sf::FloatRect gb = m_spriteCro.getGlobalBounds();
		const float halfW = gb.width / 2.f;

		const float minX = halfW;
		const float maxX = window.getSize().x - halfW;

		// ajuste patrouille si nécessaire
		if(m_leftX < minX) m_leftX = minX;
		if(m_rightX > maxX) m_rightX = maxX;

		sf::Vector2f pos = m_spriteCro.getPosition();
		if(pos.x < minX) {pos.x = minX; m_dir = -1;}
		if(pos.x > maxX) {pos.x = maxX; m_dir = +1;}

		m_spriteCro.setPosition(pos.x, pos.y);
	}

void crocodilo::draw(sf::RenderWindow& window) const 
	{
		window.draw(m_spriteCro);
	}








