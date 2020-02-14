#pragma once
#include "Sprite.h"
class background : public Sprite
{
public:
	background(const sf::Vector2f & size = sf::Vector2f(0, 0));
	~background();
	void update(float dt);
};
