#ifndef MENU_HPP
#define MENU_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include <SFML/Audio.hpp> 

class Menu
	{
		public:
			Menu(sf::RenderWindow& window);
			void draw();
			void update();
			void handleEvent(const sf::Event& event);
			bool wantsToStartGame() const;
			void setStartGameFalse() {m_startGame = false;}
			//void playDeathSound();
			void drawGameOverMenu(sf::RenderWindow& window, int hitScore);
			bool canStartDeathAnimation();
			void startMusicMenu();
			void stopMusicMenu();
		private:
			sf::RenderWindow& m_window;
			sf::Clock m_blinkClock; // horloge locale
			sf::Font m_font;
			sf::Text m_title;
			sf::Text m_startText;
			sf::Text m_name;
			bool m_startGame = false;
			float m_blinkTimer = 0.f;
			bool m_blinkState = false;
			sf::Sound m_deathSound; // son mort
			sf::SoundBuffer m_deathBuffer; // données du son
			sf::Music m_menuMusic;
	};
// méthodes hors classe
void drawPauseMenu(sf::RenderWindow& window);
void drawScore(sf::RenderWindow& window, int hitScore);

#endif