#include "Boss.h"

Boss::Boss(const sf::Vector2f & size) : AnimatedSprite(size)
{
	/*
		IDLE:
		0, 0, 41, 64, 4
		RUN :
		0, 64, 50, 56, 3
		SUMMON :
		0, 120, 44, 47, 2
		ATTACK1 :
		0, 167, 58, 63, 3
		ATTACK2 :
		0, 230, 48, 56, 3
		TELEPORT :
		0, 286, 44, 62, 4
		DEATH :
		0, 348, 46, 60, 4
		*/
	idle.init(0, 0, 41, 63, 4, 0.25f, true);
	run.init(0, 64, 50, 56, 3, 0.25f, true);
	summon.init(0, 120, 44, 47, 2, 0.25f, true);
	attack1.init(0, 167, 58, 63, 3, 0.25f, true);
	attack2.init(0, 230, 48, 56, 3, 0.25f, true);
	teleport.init(0, 286, 45, 62, 4, 0.25f, false);
	death.init(0, 348, 46, 60, 4, 0.25f, true);
	currentAnimation = &attack2;
	frame = currentAnimation->getCurrentFrame();
	enemyMovement = RUN;
	aiming = LEFT;
	setTextureRect(frame);
	moving = true;
	//flipSprite();
	health = 50;
	attackTimer = 0.0f;
	collisionTimer = 0.0f;
	hitTimer = 0.0f;
	setSize(sf::Vector2f(212, 250));
	
}

Boss::~Boss()
{
	health = 499;
	teleTimer = 0;
	punchRect.setSize(sf::Vector2f(100, 40));
	punchRect.setFillColor(sf::Color(0, 0, 255, 255));
	punchRect.setPosition(-1000, 10000);
	attackTimer = 0.0f;
	hitTimer = 0.0f;
}

void Boss::takeTarget(sf::Vector2f target)
{
	Target = target;
}

void Boss::update(float dt)
{
	sf::Vector2f BossToPlayer(Target.x - getPosition().x, 0);
	bomb.update(dt);

	if (Target.x > getPosition().x)
	{
		currentAnimation->flipped = false;
		aiming = RIGHT;
	}
	if(Target.x < getPosition().x)
	{
		//flipSprite();
		currentAnimation->flipped = true;
		aiming = LEFT;
	}


	if (health == 0)
	{
		setAlive(false);
	}

	if (health < 500)
	{
		summonTriggered = true;
		summonTimer += dt;
	}
		switch (enemyMovement)
		{
		case IDLE:
		{
			currentAnimation = &idle;
			break;
		}
		case RUN:
		{
			if (summonTriggered && summonTimer > 30.0f)
			{
				enemyMovement = TELEPORT;
			}
			if ((BossToPlayer.x * BossToPlayer.x) / abs(BossToPlayer.x) > 100)
			{
				setVelocity(BossToPlayer.x / 1.5, 0);
			}
			else
			{
				enemyMovement = PUNCH;
				setVelocity(0, 0);
			}

			currentAnimation = &run;
			break;
		}
		case PUNCH:
		{
			if ((BossToPlayer.x * BossToPlayer.x) / abs(BossToPlayer.x) > 140)
			{
				enemyMovement = RUN;
			}
			currentAnimation = &attack1;
			if (attackTimer > 0.2f)
			{
				Attack();
				attackTimer = 0;
			}
			break;
		}

		case SUMMON:
		{
			currentAnimation = &summon;
			if (summonTriggered)
			{
			bomb.setPosition(Target.x, Target.y - 200);
			summonTimer = 0.0f;
			}
			enemyMovement = RUN;
			break;
		}

		case TELEPORT:
		{
			teleTimer += dt;
			currentAnimation = &teleport;
			if (teleTimer > 0.6f)
			{
				setVelocity(0, 0);
				if (Target.x > getPosition().x)
				{
					setPosition(100, getPosition().y);
					enemyMovement = SUMMON;
					teleTimer = 0.0f;
				}
				else
				{
					setPosition(800, getPosition().y);
					enemyMovement = SUMMON;
					teleTimer = 0.0f;
				}
			}
			break;
		}

		}





	// Animation, movement, collision updating and timers are here.
	move(velocity.x*dt, velocity.y*dt);
	updateAABB();
	elapsedTime += dt;
	attackTimer += dt;
	hitTimer += dt;
	collisionTimer += dt;

	if (elapsedTime >= currentAnimation->getAnimationTime())
	{
		currentAnimation->nextFrame();
		setTextureRect(currentAnimation->getCurrentFrame());
		elapsedTime = 0;
	}

}


void Boss::swordCollisionResponse()
{
	if (hitTimer > 0.1f)
	{
		health -= 50;
		hitTimer = 0;
	}
}

void Boss::Attack()
{
	
}

void Boss::flipSprite()
{
	currentAnimation->flip();
}

