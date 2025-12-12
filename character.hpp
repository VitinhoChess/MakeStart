#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <SFML/Graphics.hpp> 
#include <SFML/Audio.hpp> 
#include <string>
#include <algorithm>

class character 
      {
            public:

			// constructeur, fonction appelée quand on déclare un character
                  character(const std::string& texturePath, const std::string& texturePathWalk, float startX, float startY);

			// méthodes
                  void update(float dt);
                  void move(float dx, float dy);
                  void draw(sf::RenderWindow& window);
			void handleInput(float dt);
			void clampToWindow(const sf::RenderWindow& window);
                  sf::FloatRect getBounds() const;
			void setPositionMan(float x, float y, int orientation);
			// hitbox
			sf::FloatRect getHitbox() const;
			void drawHitbox(sf::RenderWindow& window, sf::Color color =sf::Color::Green) const;
			void setHitboxSize(sf::Vector2f size) {m_hitboxSize = size;}
			void setHitboxOffset(sf::Vector2f offs) {m_hitboxOffset = offs;} 
			// pdv
			void setHealth(float pv) {m_pv = std::clamp(pv, 0.f, m_pvMax);}
			float getHealth() const {return m_pv;}
			float getMaxHealth() const {return m_pvMax;}
			void takeDammage(float amount);
			bool isDead() const {return m_pv <= 0.f;}
			float getHealthRatio() const {return (m_pvMax > 0.f ) ? (m_pv / m_pvMax) : 0.f;}
			void resetHealth() {m_pv = m_pvMax;}
			void setupHealthBar(const sf::Vector2f& pos, const sf::Vector2f& size);
			void drawHealthBar(sf::RenderWindow& window) const;
			// score
			void setupScore(const sf::Vector2f& pos, const sf::Vector2f& size);
			void drawScore(sf::RenderWindow& window) const;
			// animation & son mort
			//void playDeathSound();

            private:
			// textures animation
                  sf::Texture m_texture;
			sf::Texture m_textureW;

                  sf::Sprite m_sprite;

			// variables d'animation de marche
			bool m_isMoving = false;
			bool m_useAltFrame = false;
			bool m_justStartedMoving = false;
			float m_animTimer = 0.f;

			// variable de taille (affichage)
			float m_size = 0.72f;

			// variables de saut
			float m_velocityY = 0.f;
			float m_jumpTimer = 0.f;
			bool m_isOnGround = false;
			const float m_gravity = 800.f;

			// variables audio
			sf::SoundBuffer m_jumpBuffer; // données du son
			sf::Sound m_jumpSound; // joue le son

			// hitbox
			sf::Vector2f m_hitboxSize = {110.f, 75.f}; // taille hitbox
			sf::Vector2f m_hitboxOffset = {0.f, 0.f}; // décalage facultatif

			// pv
			float m_pv = 100.f;
			float m_pvMax = 100.f;
			// parametres barre de vie
			sf::Vector2f m_pvBarPos = {20.f, 20.f};
			sf::Vector2f m_pvBarSize = {200.f, 20.f};
			sf::Color m_pvBackColor = sf::Color::White;
			sf::Color m_pvFrontColor = sf::Color::Red;
			
			
      };

#endif