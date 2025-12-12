#include "menus.hpp"

Menu::Menu(sf::RenderWindow& window)
	: m_window(window)
{
	if(!m_font.loadFromFile("graphismes/regular.ttf")) 
		{
			std::cerr << "erreur : police arial.ttf" << std::endl;
		}

// ----- definition des strings apparaissant sur le menu -----
	// titre principal
	m_title.setFont(m_font);
	m_title.setString("MAKE START");
	m_title.setCharacterSize(60);
	m_title.setFillColor(sf::Color::Blue);

	// texte secondaire
	m_startText.setFont(m_font);
	m_startText.setString("Press ENTER to start");
	m_startText.setCharacterSize(20);
	m_startText.setFillColor(sf::Color::White);

	// mon nom hehe
	m_name.setFont(m_font);
	m_name.setString("-------------- Bianchi --------------");
	m_name.setCharacterSize(10);
	m_name.setFillColor(sf::Color::White);

	// centrer m_title et m_startText
	sf::FloatRect textRect = m_title.getLocalBounds();
	m_title.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	m_title.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);
	
	textRect = m_startText.getLocalBounds();
	m_startText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	m_startText.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f + 70.f);

	textRect = m_name.getLocalBounds();
	m_name.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	m_name.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f + 42.f);
// ----- fin definition des deux strings apparaissant sur le menu -----
// variables music de fond
	m_menuMusic.openFromFile("graphismes/musicTheme.wav");
	m_menuMusic.setLoop(true);
}

// dessiner et afficher les string : clear puis draw puis display
void Menu::draw() 
	{
		m_window.clear(sf::Color::Black);
		m_window.draw(m_title);
		m_window.draw(m_startText);
		m_window.draw(m_name);
		m_window.display();
	}

void Menu::handleEvent(const sf::Event& event) 
	{
		if((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)) 
			m_startGame = true;
	}


bool Menu::wantsToStartGame() const 
	{
		return m_startGame;
	}

// effet blink
void Menu::update()
	{
		float dt = m_blinkClock.restart().asSeconds();
		m_blinkTimer += dt;
		if(m_blinkTimer >= 1.8f) 
			{
				m_blinkTimer = 0.f;
				m_blinkState = !m_blinkState;

				if(m_blinkState)
					m_startText.setFillColor(sf::Color::Red);
				else
					m_startText.setFillColor(sf::Color::White);
			}
	}

//menu pause

void drawPauseMenu(sf::RenderWindow& window) 
	{
		sf::Text m_pauseText;
		sf::Font m_font;
		m_font.loadFromFile("graphismes/regular.ttf");
		
		m_pauseText.setFont(m_font);
		m_pauseText.setString("PAUSE");
		m_pauseText.setCharacterSize(40);
		m_pauseText.setFillColor(sf::Color::White);
		
		sf::FloatRect textRect = m_pauseText.getLocalBounds(); // après setSize !

		// centrer
		m_pauseText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
		m_pauseText.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);

		// overlay
		sf::RectangleShape overlay;
		overlay.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
		overlay.setFillColor(sf::Color(0, 0, 0, 120)); // transparence

		window.draw(overlay);
		window.draw(m_pauseText);
	}

void drawScore(sf::RenderWindow& window, int hitScore) 
	{
		sf::Text scoreText;
		sf::Font font;
		font.loadFromFile("graphismes/regular.ttf");
		
		scoreText.setFont(font);
		scoreText.setString(std::to_string(hitScore));
		scoreText.setCharacterSize(30);
		scoreText.setFillColor(sf::Color::White);

		sf::FloatRect textRect = scoreText.getLocalBounds(); // après setCharacterSize !

		// centrer
		scoreText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
		scoreText.setPosition(730.f, 35.f);
		
		// afficher (draw)
		window.draw(scoreText);

		
	}

void Menu::drawGameOverMenu(sf::RenderWindow& window, int finalScore) 
	{
		window.clear();

		sf::Text GameOverText;
		sf::Font font;
		font.loadFromFile("graphismes/regular.ttf");

		// game over text
		GameOverText.setFont(font);
		GameOverText.setString("GAME OVER");
		GameOverText.setCharacterSize(50);
		GameOverText.setFillColor(sf::Color::Red);

		sf::FloatRect textRect = GameOverText.getLocalBounds(); // après setCharacterSize !
		
		// centrer
		GameOverText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
		GameOverText.setPosition(400.f, 300.f);

		// score text
		sf::Text scoreText;
		scoreText.setFont(font);
		std::string str = "Your score is " + std::to_string(finalScore);
		scoreText.setString(str);
		scoreText.setCharacterSize(30);
		scoreText.setFillColor(sf::Color::White);

		// centrer
		textRect = scoreText.getLocalBounds();
		scoreText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
		scoreText.setPosition(400.f, 500.f);

		window.draw(GameOverText);
		window.draw(scoreText);
		window.display();

	}

bool Menu::canStartDeathAnimation() 
	{

		// fonction appelée à la mort de player. Joue le son de mort et renvoie true quand on peut changer de gameState
		bool canStart = false;
	
		// son game over
		if(!m_deathBuffer.loadFromFile("graphismes/death.wav"))
			std::cerr << "erreur load death.wav" << std::endl;
		m_deathSound.setBuffer(m_deathBuffer);
		m_deathSound.setVolume(100.f); // volume son 0 à 100
		m_deathSound.setPitch(1.f); // aigu (fréquence et durée) 0.5 grave, 2.0 aigu

		bool deathSoundPlayed = false;
		m_deathSound.play();
		deathSoundPlayed = true;

		if(deathSoundPlayed && m_deathSound.getStatus() == sf::Sound::Stopped) 
			{
				canStart = true;
				return canStart;
			}
		return canStart;
	}

void Menu::startMusicMenu() 
	{
		if(m_menuMusic.getStatus() != sf::Sound::Playing)
			m_menuMusic.play();
	}

void Menu::stopMusicMenu() 
	{
		m_menuMusic.stop();
	}





