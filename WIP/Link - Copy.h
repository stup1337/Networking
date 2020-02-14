#pragma once
#include "AnimatedSprite.h"
#include "Animation.h"
#include "Input.h"
#include "Vector.h"

class Link : public AnimatedSprite
{
public:
	Link(const sf::Vector2f &size = sf::Vector2f(0, 0));
	~Link();
	void update(float dt);
	void flipSprite();
	bool animating = true;
	float scale;
	float speed, acceleration;
	sf::Vector2f target;
	sf::Vector2f direction;
	bool moving;
	void takeInput(Input* input);
protected : 
	Animation walkDown;
	Animation walkLeft;
	Animation walkUp;
	Animation* currentAnimation;
};

