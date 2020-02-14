#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "Input.h"
#include <windows.h>
#include <string.h>
#include "StaticSprite.h"
#include "gameStates.h"
#include "AudioManager.h"

struct Decision {
	bool decisionMade;
	bool isServer;
};

class Menu {
public:
	Menu(sf::RenderWindow* hwnd, Input* input);
	~Menu();
	void handleInput(float dt, Input* in);
	void update(float dt);
	void render();
	sf::Sprite bg;
	sf::Texture bgText;
	float timer = 0.0f;
	bool madeDecision = false;
	bool isServer = false;
	Decision CheckDecision();
private:
	sf::RenderWindow* window;
	Input* input;
	void beginDraw();
	void endDraw();
};