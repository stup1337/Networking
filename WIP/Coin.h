#pragma once
#include "AnimatedSprite.h"
#include "StaticSprite.h"
#include "Animation.h"
#include "Input.h"
#include <cmath>   
#include "AudioManager.h"

class Coin : public AnimatedSprite
{
public:
	Coin(const sf::Vector2f &size = sf::Vector2f(0, 0));
	~Coin();
	void update(float dt);
	void flipSprite();
	float scale;
	float tempPos;
	float friction;
protected:
	Animation spin;
	Animation* currentAnimation;
};

