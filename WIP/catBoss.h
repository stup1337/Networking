#pragma once
#include "AnimatedSprite.h"
#include "StaticSprite.h"
#include "Animation.h"
#include "Input.h"
#include "Vector.h"
#include <cmath>   
#include "AudioManager.h"

class catBoss : public AnimatedSprite
{
public:
	catBoss(const sf::Vector2f &size = sf::Vector2f(0, 0));
	~catBoss();
	void update(float dt);
	void flipSprite();
	void Attack();
	void wallCollision();
	void takeTarget(sf::Vector2f target);
	float scale;
	float tempPos;
	float angle = 0.0;
	float angle2 = 0.0f;
	float friction;
	bool hitWall;
	float Distance = 0.0f;
	float collisionTimer;
	std::vector<StaticSprite> fireballs;
	uint32_t health = 10;
	float tempDistance = 0.0f;
	sf::RectangleShape swordRect;
	sf::Vector2f Target;
	sf::Vector2f direction;
	float attackTimer;
	enum MovementState {PENDULUM,SPITFIRE};
	MovementState enemyMovement;
	void swordCollisionResponse();
	float hitTimer;
	bool moving;
protected:
	Animation walk;
	Animation* currentAnimation;
};

