#ifndef MISSILE_HPP
#define MISSILE_HPP

#include <SFML/Graphics.hpp> 
#include <SFML/Audio.hpp> 
#include <string>

class missile 
	{
		public:
			explicit missile(const sf::Texture& texture);
			void launch(sf::Vector2f startPos, float speed, float angleDeg);
			void update(float dt, const sf::RenderWindow& window);
			void draw(sf::RenderWindow& window) const;
			bool isAlive() const {return m_alive;}
			bool intersects(const sf::FloatRect& other) const;
			void invertAlive();
			void setExplosionTexture(const sf::Texture* t0, const sf::Texture* t1, const sf::Texture* t2) 
				{
					m_expl[0] = t0; m_expl[1] = t1; m_expl[2] = t2;
				}
			void triggerExplosion();
			missile(missile&&) noexcept = default;
			missile& operator=(missile&&) noexcept = default;
			missile(const missile&) = delete;
			missile& operator=(const missile&) = delete;
			// hitbox
			sf::FloatRect getHitbox() const;
			void drawHitbox(sf::RenderWindow& window, sf::Color color = sf::Color::Red) const;
			void setHitboxSize(sf::Vector2f s) {m_hitboxSize = s;}
			void setHitboxOffset(sf::Vector2f o) {m_hitboxOffset = o;}
			// dégats
			float getDammage() const {return m_damage;}
			void setDammage(float d) {m_damage = d;}
			bool canTakeDamage() const {return m_alive && !m_hasHit;}
			void markHasHit() {m_hasHit = true;}
			// detection hit ground (pour le score)
			bool hasHitGround() const  {return m_hasHitGround;}
			bool isGroundCounted() const  {return m_groundCounted;}
			void markGroundCounted()  {m_groundCounted = true;}

		private:
			const sf::Texture* m_tex = nullptr;
			sf::Sprite m_sprite;
			sf::Vector2f m_vel; // vitesse en x et y
			bool m_alive = false;
			bool m_exploding = false;
			int m_explIndex = 0;
			float m_explTimer = 0.f;
			float m_explFrameDt = 0.3f; // ms par frame
			const sf::Texture* m_expl[3] = {nullptr, nullptr, nullptr};
			static float deg2grad(float d) {return d * 3.1415926535f / 180.f;} // conversion maths
			void applyTextureKeepCenter(const sf::Texture& tex);
			sf::SoundBuffer m_explBuffer; // données du son
			sf::Sound m_explSound; // joue le son
			//hit box
			sf::Vector2f m_hitboxSize = {30.f, 80.f};
			sf::Vector2f m_hitboxOffset = {0.f, 0.f};
			// dégats
			float m_damage = 25.f;
			bool m_hasHit = false;
			bool m_hasHitGround;
			bool m_groundCounted;

	};

#endif