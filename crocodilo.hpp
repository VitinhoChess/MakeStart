#ifndef CROCODILO_HPP
#define CROCODILO_HPP

#include <SFML/Graphics.hpp>
#include <string>


class crocodilo  // nom de classe type ia
	{
		public:
		crocodilo(const std::string& texturePath, float y, float leftX, float rightX, float speed); // constructeur

		void update(float dt);
		void clampToWindow(const sf::RenderWindow&);
		void draw(sf::RenderWindow& window) const;

		sf::Vector2f getPosition() const {return m_spriteCro.getPosition();}
		sf::FloatRect getBounds() const {return m_spriteCro.getGlobalBounds();}

		void setSpeed(float s) {m_speed = s;}
		void setPatrol(float leftX, float rightX) {m_leftX = leftX; m_rightX = rightX;}

		void setOscillation(float amp, float freq) {m_oscAmp = amp; m_oscFreq = freq;}
		void setTurbulence(float amp, float freq) {m_turbAmp = amp; m_turbFreq = freq;}

		private:
		sf::Texture m_textureCro;
		sf::Sprite m_spriteCro;

		float m_leftX;
		float m_rightX;
		float m_speed;
		float m_altitude;
		float m_size = 0.3f;
		int m_dir = -1;

		float m_oscAmp;
		float m_oscFreq;
		float m_turbAmp;
		float m_turbFreq;
		float m_time;
	};

#endif






