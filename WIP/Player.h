#pragma once
#include "AnimatedSprite.h"
#include "Animation.h"
#include "Input.h"
#include <cmath>   
#include "StaticSprite.h"
#include "gameStates.h"

class Player : public AnimatedSprite
{
public:
	Player(const sf::Vector2f &size = sf::Vector2f(0, 0));
	~Player();
	void SetPlayer2(bool player);
	void update(float dt);
	void Attack();
	void SwordAndShieldResponse();
	void TakeDamage(float damage);
	bool animating = true;
	void Animate(float dt);
	StaticSprite HealthBar;
	StaticSprite StaminaBar;
	const int maxHealth = 1000.0f;
	int health = 1000.0f;
	float friction = 20.0f;
	float attackTimer;
	float hitTimer;
	float stamina;
	float maxStamina;
	float attackTime = 1.0f;
	float blockTimer;
	bool player2;
	float blockTime = 1.0f;
	sf::Texture slashText;
	sf::Texture healthBarText;
	bool vulnerable = false;
	sf::RectangleShape swordRect;
	sf::RectangleShape shieldRect;
	MovementState playerMovement;
	MovingDirection playerDirection;
	//void ApplyFriction(float friction);
	void ApplyFriction(float friction, float dt);
	void takeInput(Input* input);
protected : 
	Animation idleAnimation;
	Animation walkAnimation;
	Animation attackAnimation;
	Animation deathAnimation;
	Animation blockAnimation;
	Animation* currentAnimation;
};

