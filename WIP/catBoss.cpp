#include "catBoss.h"





catBoss::catBoss(const sf::Vector2f & size) : AnimatedSprite(size)
{
	walk.init(0, 0, 128, 128, 2, 0.25f, true);
	currentAnimation = &walk;
	frame = currentAnimation->getCurrentFrame();
	enemyMovement = PENDULUM;
	setTextureRect(frame);
	health = 100;
	moving = true;
	scale = 200.0;
	health = 50;
	attackTimer = 0.0f;
	collisionTimer = 0.0f;
	hitTimer = 0.0f;
	angle = 0;
}

catBoss::~catBoss()
{

}

void catBoss::takeTarget(sf::Vector2f target)
{
	Target = target;
}

void catBoss::update(float dt)
{
	switch(enemyMovement)
	{
		case PENDULUM:
		{
			angle += 0.8 * dt;
			angle2 += 1.2 * dt;

			if (fireballs[0].isAlive())
			{
				fireballs[0].setPosition(getPosition().x + cos(angle) * 300 + 100, getPosition().y + sin(angle) * 300 - 20);
				fireballs[0].update(dt);
			}
			if (fireballs[1].isAlive())
			{
				fireballs[1].setPosition(getPosition().x + cos(angle) * 200 + 100, getPosition().y + sin(angle) * 200 - 20);
				fireballs[1].update(dt);
			}
			if (fireballs[2].isAlive())
			{
				fireballs[2].setPosition(getPosition().x + cos(angle) * 100 + 100, getPosition().y + sin(angle) * 100 - 20);
				fireballs[2].update(dt);
			}
			if (fireballs[3].isAlive())
			{
				fireballs[3].setPosition(getPosition().x + cos(angle2) * 300 + 100, getPosition().y + sin(angle2) * 300 - 20);
				fireballs[3].update(dt);
			}
			if (fireballs[4].isAlive())
			{
				fireballs[4].setPosition(getPosition().x + cos(angle2) * 200 + 100, getPosition().y + sin(angle2) * 200 - 20);
				fireballs[4].update(dt);
			}
			if (fireballs[5].isAlive())
			{
				fireballs[5].setPosition(getPosition().x + cos(angle2) * 100 + 100, getPosition().y + sin(angle2) * 100 - 20);
				fireballs[5].update(dt);
			}

			if (attackTimer > 10)
			{
				for (int i = 6; i < 14; i++)
				{
					fireballs[i].setAlive(true);
					fireballs[i].setPosition(600,450);
					fireballs[i].setVelocity(0, 0);
					attackTimer = 0;
				}
				enemyMovement = SPITFIRE;
			}
			break;
		}
		case SPITFIRE:
		{
			
			if (fireballs[6].isAlive())
			{
				fireballs[6].setVelocity(-120, -120);
				fireballs[6].update(dt);
			}
			if (fireballs[7].isAlive())
			{
				fireballs[7].setVelocity(0, -120);
				fireballs[7].update(dt);
			}
			if (fireballs[8].isAlive())
			{
				fireballs[8].setVelocity(120, -120);
				fireballs[8].update(dt);
			}

			if (fireballs[9].isAlive())
			{
				fireballs[9].setVelocity(120, 0);
				fireballs[9].update(dt);
			}
			if (fireballs[10].isAlive())
			{
				fireballs[10].setVelocity(120, 120);
				fireballs[10].update(dt);
			}

			if (fireballs[11].isAlive())
			{
				fireballs[11].setVelocity(0, 120);
				fireballs[11].update(dt);
			}

			if (fireballs[12].isAlive())
			{
				fireballs[12].setVelocity(-120, 120);
				fireballs[12].update(dt);
			}

			if (fireballs[13].isAlive())
			{
				fireballs[13].setVelocity(-120, 0);
				fireballs[13].update(dt);
			}

			if (attackTimer > 7)
			{
				enemyMovement = PENDULUM;
				for(int i = 0; i < 6; i++)
				{
					fireballs[i].setAlive(true);
					fireballs[i].setPosition(sf::Vector2f(550,450));
					attackTimer = 0;
				}
			}

			break;
		}
	}

	if (health == 0)	// hide fireballs upon death.
	{
		for (int i = 0; i < fireballs.size(); i++)
		{
			fireballs[i].setAlive(false);
		}
		setAlive(false);
	}

	if (health > 100)
	{
		health = 100;
	}

	currentAnimation = &walk;
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

void catBoss::wallCollision()
{
}

void catBoss::swordCollisionResponse()
{	
	//stops continous damage by putting a timer on it.
	if (hitTimer > 1.5)
	{
		health -= 5;
		hitTimer = 0;
	}
}




void catBoss::Attack()
{
}

void catBoss::flipSprite()
{
	currentAnimation->flip();
}

