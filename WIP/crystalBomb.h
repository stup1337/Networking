#pragma once
#include "AnimatedSprite.h"
#include "StaticSprite.h"
#include "Animation.h"
#include "Input.h"
#include <cmath>   
#include "AudioManager.h"

class crystalBomb : public AnimatedSprite
{
public:
	crystalBomb(const sf::Vector2f &size = sf::Vector2f(0, 0));
	~crystalBomb();
	void update(float dt);
	void playerCollision();
	void flipSprite();
	void wallCollision();
	void takeTarget(sf::Vector2f target);
	float scale;
	float tempPos;
	float friction;
	float collisionTimer;
	float hitWallTimer;
	bool hitWallTest;
	uint32_t health = 10;
	sf::Vector2f Target;
	sf::Vector2f direction;
	float attackTimer;
	void swordCollisionResponse();
	float hitTimer;
	float randomTimer;
protected:
	Animation rise;
	Animation* currentAnimation;
};

