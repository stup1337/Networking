#include "Player.h"

Player::Player(const sf::Vector2f & size) : AnimatedSprite(size)
{
	/*walkDown.init(0, 0, 32, 32, 4, 0.25f,true);
	walkLeft.init(0, 32, 32, 32, 4, 0.10f, true);
	walkRight.init(0, 64, 32, 32, 4, 0.10f,true);
	walkUp.init(0, 96, 32, 32, 4, 0.25f,true);
	swordDown.init(0, 128, 32, 32, 4, 0.10f,false);
	swordLeft.init(0, 160, 32, 32, 4, 0.10f,false);
	swordRight.init(0, 192, 32, 32, 4, 0.10f,false);
	swordUp.init(0, 224, 32, 32, 4, 0.10f,false);*/
	walkAnimation.init(0, 0, 42, 42, 8, 0.2f, true);
	idleAnimation.init(0, 84, 42, 42, 4, 0.2f, true);
	attackAnimation.init(0, 168, 42, 42, 10, 0.5f, true);
	deathAnimation.init(0, 42, 40, 40, 9, 0.3f, false);
	blockAnimation.init(0, 131, 42, 42, 7, 0.2f, false);
	currentAnimation = &idleAnimation;
	frame = currentAnimation->getCurrentFrame();
	playerMovement = MovementState::IDLE;
	setTextureRect(frame);
	swordRect.setSize(sf::Vector2f(110,120));
	swordRect.setPosition(10000, 1000);
	swordRect.setFillColor(sf::Color::Red);
	shieldRect.setSize(sf::Vector2f(140, 168));
	shieldRect.setPosition(10000, 1000);
	shieldRect.setFillColor(sf::Color::Green);
	healthBarText.loadFromFile("Images/healthBar.png");
	HealthBar.setTexture(&healthBarText);
	HealthBar.setPosition(sf::Vector2f(100, 20));
	HealthBar.setSize(sf::Vector2f(100, 100));
	StaminaBar.setTexture(&healthBarText);
	StaminaBar.setPosition(sf::Vector2f(150, 50));
	StaminaBar.setSize(sf::Vector2f(100, 100));
	StaminaBar.setFillColor(sf::Color(200, 0, 255, 255));
	StaminaBar.setOutlineColor(sf::Color(0, 255, 0, 255));
	maxStamina = 100.0f;
	stamina = maxStamina;
	hitTimer = 0.0f;
	attackTimer = 0.0f;
}

Player::~Player()
{
}

void Player::SetPlayer2(bool player)
{
	player2 = player;
	HealthBar.setPosition(700, 20);
	StaminaBar.setPosition(750, 50);
	setPosition(1000, 450);
	playerDirection = MovingDirection::LEFT;
	currentAnimation->flip();
}
void Player::update(float dt)
{
	// Timers
	hitTimer += dt;
	blockTimer += dt;
	attackTimer += dt;
	ApplyFriction(friction, dt);
	if (currentAnimation == &attackAnimation)
	{
		setVelocity(0, 0);
		attackTimer += dt;
		if (attackTimer > attackTime)
		{
			playerMovement = MovementState::IDLE;
			attackTimer = 0;
		}
	}
	if (velocity.x == 0)
	{
		currentAnimation = &idleAnimation;
	}
	updateAABB();
	move(velocity.x*dt, velocity.y*dt);
	HealthBar.setSize(sf::Vector2f((health * 400) / maxHealth, 20));	// Updates the health bar size based on the percentage of health remaining. Spent far too long on getting the bodmas right -__-
	StaminaBar.setSize(sf::Vector2f((stamina * 200) / maxStamina, 20));

#pragma region Health/Stamina
	if (health == 0)
	{
		setAlive(false);
	}
	if (stamina <= 0.1)
	{
		stamina = 0.1;
		if (playerMovement == MovementState::ATTACK || (playerMovement == MovementState::BLOCK))
		{
			vulnerable = true;
		}
	}
	else
	{
		if (stamina != maxStamina)
		{
			if (!vulnerable)
			{
				if (!(playerMovement == MovementState::ATTACK || (playerMovement == MovementState::BLOCK)))
				{
					stamina += dt * 4;
				}
			}
		}
	}
	
	if (stamina > 20)
	{
		vulnerable = false;
	}
	if (stamina > maxStamina)
	{
		stamina = maxStamina;
	}

	if (health > maxHealth)
	{
		health = maxHealth;
	}
#pragma endregion Stamina/Health boundary checking

	// Player Controls
	switch (playerMovement)
	{
		// Player case movement statement : Tells update what to do in the various scenarios where the player clicks or is moving.
		case MovementState::IDLE:
		{
			SwordAndShieldResponse();
			//setVelocity(0, -158);
			currentAnimation = &idleAnimation;
			break;
		}

		case MovementState::WALK:
		{
			SwordAndShieldResponse();
			currentAnimation = &walkAnimation;
			break;
		}

		case MovementState::ATTACK:
		{
			Attack();
			currentAnimation = &attackAnimation;
			if (attackTimer < 24.8f)
			{
				playerMovement = MovementState::IDLE;
			}
			break;
		}

		case MovementState::DEATH:
		{
			setSize(sf::Vector2f(256, 256));
			currentAnimation = &deathAnimation;
			break;
		}

		case MovementState::BLOCK:
		{
			if (stamina > 2)
			{
				stamina -= dt * 15;
				if (playerDirection == MovingDirection::LEFT)
				{
					shieldRect.setPosition(getPosition().x + 50, getPosition().y);
				}
				else
				{
					shieldRect.setPosition(getPosition().x + 80, getPosition().y);
				}
				setSize(sf::Vector2f(256, 256));
				currentAnimation = &blockAnimation;
			}
			else
			{
				playerMovement = MovementState::IDLE;
			}
			break;
		}
	}
	// Player Directional States, having to switch animations for each part.
	switch (playerDirection)
	{
		case MovingDirection::LEFT:
		{
			setVelocity(velocity.x, 0);
			break;
		}
		case MovingDirection::RIGHT:
		{
			setVelocity(velocity.x, 0);
			break;
		}
	}
	Animate(dt);
}


void Player::ApplyFriction(float friction, float dt)
{
		if (velocity.x > 0)
		{
			if (velocity.x != 0)
			{
				velocity.x -= friction;
			}
		}
		if (velocity.x < 0)
		{
			if (velocity.x != 0)
			{
				velocity.x += friction;
			}
		}
}
void Player::takeInput(Input* input)
{
		// changes state based on controls of WSAD for movement.
		if (input->isKeyDown(sf::Keyboard::D))
		{
			input->setKeyUp(sf::Keyboard::D);
			setVelocity(500, 0);
			playerDirection = MovingDirection::RIGHT;
			walkAnimation.flipped = false;
			playerMovement = MovementState::WALK;
		}

		if (input->isKeyDown(sf::Keyboard::A))
		{
			input->setKeyUp(sf::Keyboard::A);
			setVelocity(-500, 0);
			playerDirection = MovingDirection::LEFT;
			walkAnimation.flipped = true;
			playerMovement = MovementState::WALK;
		}
		if (input->isKeyDown(sf::Keyboard::Q))
		{
			input->setKeyUp(sf::Keyboard::Q);
			playerMovement = MovementState::ATTACK;
			if (playerDirection == MovingDirection::RIGHT)
			{
				attackAnimation.flipped = false;
			}
			else
			{
				attackAnimation.flipped = true;
			}
		}

		if (input->isKeyDown(sf::Keyboard::E))
		{
			input->setKeyUp(sf::Keyboard::E);
			if (playerDirection == MovingDirection::RIGHT)
			{
				blockAnimation.flipped = false;
			}
			else
			{
				blockAnimation.flipped = true;
			}
			playerMovement = MovementState::BLOCK;
		}
}

void Player::Animate(float dt)
{
	elapsedTime += dt;

	if (elapsedTime >= currentAnimation->getAnimationTime())
	{
		currentAnimation->nextFrame();
		setTextureRect(currentAnimation->getCurrentFrame());
		elapsedTime = 0;
	}
}
void Player::Attack()
{
	if (playerDirection == MovingDirection::LEFT)
	{
		if (attackTimer > 0.8f)
		{
			swordRect.setPosition(getPosition().x, getPosition().y + 50);
			attackTimer = 0.0f;
		}
	}

	if (playerDirection == MovingDirection::RIGHT)
	{
		if (attackTimer > 0.8f)
		{
			swordRect.setPosition(getPosition().x + 150, getPosition().y + 50);
			attackTimer = 0.0f;
		}
	}
	
}


void Player::SwordAndShieldResponse()
{
	shieldRect.setPosition(100000, 1000);
	swordRect.setPosition(10504, 102);
}

void Player::TakeDamage(float damage)
{
	if (hitTimer > 0.2f)
	{
		if (health > damage)
		{
			health -= damage;
			hitTimer = 0.0f;
		}
		else
		{
			hitTimer = 0.0f;
			health = 0.1f;
			Alive = false;
		}
	}
	
}


