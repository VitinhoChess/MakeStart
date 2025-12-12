#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <random>
#include <vector>
#include "character.hpp"
#include "menus.hpp"
#include "crocodilo.hpp"
#include "missile.hpp"

enum class GameState 
	{
		MENU,
		GAME,
		PAUSE, 
		GAME_OVER
	};

int main()  
      
      {
		// pour le rand()
		srand(time(nullptr));
		
            // Crée une fenêtre de 800x600 pixels
            sf::RenderWindow window(sf::VideoMode(800, 600), "fenetre");

            // horloge
            sf::Clock clock;

		// Mettre le menu en premier
		GameState state = GameState::MENU;
		
		// appel constructeur menu
		Menu menu(window);

            // créer character : appel au contructeur
		// start de player
		float xStart = 400.f;
		float yStart = 600.f;

		// compteur missiles
		int hitScore = 0;
		int finalScore = 0;

		//character player("graphismes/knight.png", "graphismes/knightW.png", xStart, yStart);
      	character player("graphismes/tralalero.png", "graphismes/tralaleroW.png", xStart, yStart);

		// créer crocodilo : appel constructeur
		crocodilo bot("graphismes/crocodiloV2.png", 120.f, 60.f, 740.f, 120.f);
		bot.setOscillation(5.f, 1.f);
		bot.setTurbulence(4.f, 1.5f);

		// MISSILES
		//texture
		// mettre dans une fonction qu'on appelle ?
		sf::Texture texMissile; // def ici pour eviter de load plein de fois
		sf::Texture texExpl0;
		sf::Texture texExpl1;
		sf::Texture texExpl2;
		if(!texMissile.loadFromFile("graphismes/missilePixel.png"))
			std::cerr << "erreur load missile\n";
		if(!texExpl0.loadFromFile("graphismes/boom1pixel.png"))
			std::cerr << "erreur load explosion 1\n";
		if(!texExpl1.loadFromFile("graphismes/boom2pixel.png"))
			std::cerr << "erreur load explosion 2\n";
		if(!texExpl2.loadFromFile("graphismes/boom3pixel.png"))
			std::cerr << "erreur load explosion 3\n";
		
		std::vector<missile> missiles; // conteneur dynamique
		sf::Clock missileClock; // clock missile
		float speed = 300.f; // vitesse missile
		float nextShot = 5.f; // timer de tir
		auto randAngle = []() // lambda
			{
				int missileType = 0 + rand() % (4 + 1); // [0; 5], 0 = horizontale par convention
				int angle;

				if(missileType == 0) // plage verticale
					{
						if(rand() % 2 == 0) 
							{
								angle = 3 + rand() % (12 + 1); // [3; 15] horizontale 1
								//std::cout << " horizontal\n";
							}
						else 
							{
								angle = 165 + rand() % (12 + 1); // [165; 177] horizontale 2
								//std::cout << " horizontal\n";
							}
					}
				else // plage verticale
					{
						angle = 20 + rand() % (140 + 1); // [20; 160] vertical
						//std::cout << " vertical \n";
					}

				return angle;
			};

// ----------BOUCLE PRINCIPALE----------
            while(window.isOpen()) 
                  {
                        sf::Event event; // var event de type Event
                        while(window.pollEvent(event)) // liste des événements
                              {
                                    if(event.type == sf::Event::Closed)
                                          window.close();
                                    
                                    if(event.type == sf::Event::KeyPressed)
                                          {
                                                if(event.key.code == sf::Keyboard::Q)
                                                      window.close();
								if(event.key.code == sf::Keyboard::P) 
									{
										if(state == GameState::GAME)
											state = GameState::PAUSE;
										else if(state == GameState::PAUSE) 
											{
												state = GameState::GAME;
												clock.restart(); // restart le clock ?? verifier et debuguer
											}
									}
                                          }
						if(state == GameState::MENU) 
							{
								menu.handleEvent(event);
								menu.startMusicMenu();
							}
						
						if(state == GameState::GAME_OVER) 
							{
								if(event.type == sf::Event::KeyPressed) 
									{
										if(event.key.code == sf::Keyboard::Space) 
											{
												state = GameState::MENU;
											}
									}
							}

                              }

				if(state == GameState::MENU) 
					{
						if(menu.wantsToStartGame())
							{
								// initialisation
								state = GameState::GAME;
								menu.stopMusicMenu();
								clock.restart();
								player.resetHealth();
								missiles.clear(); // doublon sécurité (déjà dans le if(isDead))
								menu.setStartGameFalse(); // 
								
							}
						menu.update();
						menu.draw();
					}
				else if (state == GameState::GAME) 
					{
						
				
		                        // calcul delta temps en s
		                        float dt = clock.restart().asSeconds();
		
	// ----- appel fonction déplacements -----        
						player.handleInput(dt); // pas d'input pour le bot (logique)
		
		                        // updates
		                        player.update(dt);
						player.clampToWindow(window);
		                        bot.update(dt);
						bot.clampToWindow(window);

						//  INCREASE DIFFICULTY
						if(hitScore > 5)
							{
								nextShot = 3.f;
								speed = 480.f;
							}
						if(hitScore > 11) 
							{
								nextShot = 1.5f;
								speed = 480.f;
							}
						if(hitScore > 21) 
							{
								nextShot = 1.f;
								speed = 500.f;
							}
						
						// tir auto crocodilo
						if(missileClock.getElapsedTime().asSeconds() >= nextShot) 
							{
								missileClock.restart();
								
								missiles.emplace_back(texMissile);
								missiles.back().setExplosionTexture(&texExpl0, &texExpl1, &texExpl2);;
								missiles.back().launch(bot.getPosition(), speed, randAngle());
								
							}
						// maj missiles
						for(auto& m : missiles)
							{
								m.update(dt, window);
								// detection colision missile / joueur
								if(m.canTakeDamage() && m.getHitbox().intersects(player.getHitbox())) 
									{
										std::cout << "touché !\n";
										m.markHasHit();
										m.triggerExplosion();
										player.takeDammage(m.getDammage());

										// gestion mort player
										//if(player.isDead()) ancienne condition moins robuste
										if(state == GameState::GAME && player.isDead())
											{
												std::cout << "GAME OVER\n";
												
												// resets
												player.resetHealth(); // PV
												missiles.clear(); // effacer missiles
												player.setPositionMan(xStart, yStart, -1); // position
												finalScore = hitScore; // buffer
												hitScore = 0;
												speed = 300.f; // vitesse missile
												nextShot = 5.f; // timer de tir
												state = GameState::GAME_OVER;
												if(menu.canStartDeathAnimation()) 
													{
														state = GameState::GAME_OVER;
														std::cout << "BOUCLE IF\n";
													}
											}
									}
								// detection colision missile / sol
								if(m.hasHitGround() && !m.isGroundCounted())
									{	
										hitScore++;
										m.markGroundCounted();
									}
							}

						
						// suppresion missiles morts if(m_alive = false)
						missiles.erase(std::remove_if(missiles.begin(), missiles.end(), [](const missile& m){return !m.isAlive();}), missiles.end());
						
		                        window.clear(sf::Color::Black); // clear après les updates. Changer la couleur du fond ici
		                        player.draw(window);
						player.drawHealthBar(window);

						for (const auto& m : missiles)
							m.draw(window);
		                        bot.draw(window);

						bool displayHitBox = false; // afficher hitbox
						if(displayHitBox) 
							{
								player.drawHitbox(window, sf::Color::Green);
								for(const auto& m : missiles)
									m.drawHitbox(window, sf::Color::Red);
							}
					
						// AFFICHAGE SCORE
						drawScore(window, hitScore);

						// dernier
		                        window.display();
					}
				else if(state == GameState::PAUSE) 
					{
						window.clear(sf::Color::Black);
						// redessiner la dernière scène
						player.draw(window);
						for(const auto& m : missiles)
							m.draw(window);
						bot.draw(window);
						drawScore(window, hitScore);
						drawPauseMenu(window);

						window.display();
					}
				else if(state == GameState::GAME_OVER) 
					{
						menu.drawGameOverMenu(window, finalScore);
					}
                        
                  }
// ----------FIN BOUCLE PRINCIPALE----------

            return 0;
      }
