#include "Game.h"
#include "Input.h"
#include "gameStates.h"
#include <SFML/Network.hpp>
#include <cstddef>
#include <iostream>
#include "Menu.h"



void main(int argc, char** argv[])
{
	sf::RenderWindow window(sf::VideoMode(1200,800), "Networking Project");
	window.setFramerateLimit(60);
	Input input;
	bool returned = false;
	Game game(&window, &input);
	bool Check = false;
	Menu menu(&window, &input);
	srand(time(NULL));
	GameState myState = GameState::MENU;
	Decision decis;
	decis.decisionMade = false;
	decis.isServer = false;
	sf::Clock clock;
	float deltaTime;
	Decision isServer;
	while (window.isOpen())
	{
		sf::Event event;
		sf::Sprite sprite;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::Resized:
				window.setView(sf::View(sf::FloatRect(0, 0,
					event.size.width, event.size.height)));
				break;
			case sf::Event::KeyPressed:
				input.setKeyDown(event.key.code);
				break;
			case sf::Event::KeyReleased:
				input.setKeyUp(event.key.code);
				break;
			case sf::Event::MouseButtonReleased:
					if (event.mouseButton.button == sf::Mouse::Left)
					{
						input.setMouseLeftDown(false);
					}
					if (event.mouseButton.button == sf::Mouse::Right)
					{
						input.setMouseRightDown(false);
					}
				break;
			case sf::Event::MouseButtonPressed:
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					input.setMouseLeftDown(true);
				}
				if (event.mouseButton.button == sf::Mouse::Right)
				{
					input.setMouseRightDown(true);
				}
				break;
			case sf::Event::MouseMoved:
				input.setMousePosition(event.mouseMove.x, event.mouseMove.y);
				break;
			default:
				// don't handle other events
				break;
			}
			
		}
		if (input.isKeyDown(sf::Keyboard::Escape))
		{
			window.close();
		}
		deltaTime = clock.restart().asSeconds();
		switch (myState) {
		case(GameState::LEVEL):
		{
			game.handleInput(deltaTime, &input);
			game.update(deltaTime);
			game.render();
			break;
		}
		case(GameState::MENU):
		{
			menu.handleInput(deltaTime, &input);
			menu.update(deltaTime);
			menu.render();
			decis = menu.CheckDecision();
			if (decis.decisionMade)
			{
				game.Receive(decis.isServer);
				myState = GameState::LEVEL;
			}
			//myState = game.getState();
			break;
		}
		}

	}
}