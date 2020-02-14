#include "crystalBomb.h"

crystalBomb::crystalBomb(const sf::Vector2f & size) : AnimatedSprite(size)
{
	rise.init(0, 0, 75, 185, 2, 0.25f, true);
	currentAnimation = &rise;
	frame = currentAnimation->getCurrentFrame();
	setTextureRect(frame);
	collisionTimer = 0.0f;
	Alive = true;
}

crystalBomb::~crystalBomb()
{

}

void crystalBomb::takeTarget(sf::Vector2f target)
{
	Target = target;
}

void crystalBomb::update(float dt)
{
	elapsedTime += dt;

	if (elapsedTime >= currentAnimation->getAnimationTime())
	{
		currentAnimation->nextFrame();
		setTextureRect(currentAnimation->getCurrentFrame());
		elapsedTime = 0;
	}
	

}

void crystalBomb::playerCollision()
{
	if (isAlive())
	{
		Alive = !Alive;
	}
}


void crystalBomb::flipSprite()
{
	currentAnimation->flip();
}

