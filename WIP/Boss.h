#pragma once
#include "AnimatedSprite.h"
#include "StaticSprite.h"
#include "crystalBomb.h"
#include "Animation.h"
#include "Input.h"
#include <cmath>   
#include "AudioManager.h"

class Boss : public AnimatedSprite
{
public:
	Boss(const sf::Vector2f &size = sf::Vector2f(0, 0));
	~Boss();
	void update(float dt);
	void flipSprite();
	void Attack();
	void takeTarget(sf::Vector2f target);
	float friction;
	float Distance = 0.0f;
	float teleTimer = 0.0f;
	float summonTimer = 0.0f;
	float collisionTimer;
	uint32_t health = 1000;
	bool summonTriggered = false;
	float tempDistance = 0.0f;
	sf::RectangleShape punchRect;
	sf::Vector2f Target;
	sf::Vector2f direction;
	crystalBomb bomb;
	float attackTimer;
	enum aimDirectionState { LEFT, RIGHT };
	enum MovementState {IDLE, RUN, PUNCH, KICK, DEATH, SUMMON, TELEPORT};
	MovementState enemyMovement;
	aimDirectionState aiming = RIGHT;
	void swordCollisionResponse();
	float hitTimer;
	bool moving;
protected:
	Animation idle;
	Animation run;
	Animation attack1;
	Animation attack2;
	Animation death;
	Animation summon;
	Animation teleport;
	Animation* currentAnimation;
};

