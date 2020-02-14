#include "Coin.h"

Coin::Coin(const sf::Vector2f & size) : AnimatedSprite(size)
{
	spin.init(0, 0, 32, 32, 8, 0.25f, true);
	currentAnimation = &spin;
	frame = currentAnimation->getCurrentFrame();
	setTextureRect(frame);
	scale = 200.0;
}

Coin::~Coin()
{

}


void Coin::update(float dt)
{
	currentAnimation = &spin;
	move(velocity.x*dt, velocity.y*dt);
	updateAABB();
	elapsedTime += dt;

	if (elapsedTime >= currentAnimation->getAnimationTime())
	{
		currentAnimation->nextFrame();
		setTextureRect(currentAnimation->getCurrentFrame());
		elapsedTime = 0;
	}
}

void Coin::flipSprite()
{
	currentAnimation->flip();
}

