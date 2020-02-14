#pragma once
#include "Sprite.h"
#include "Input.h"

class cursor : public Sprite
{
public:
	cursor(const sf::Vector2f & size = sf::Vector2f(0, 0));
	~cursor();
	void update(float dt);
	void takeInput(Input* input);
};
