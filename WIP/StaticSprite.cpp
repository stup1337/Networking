#include "StaticSprite.h"



StaticSprite::StaticSprite(const sf::Vector2f & size) : Sprite(size)
{
}


StaticSprite::~StaticSprite()
{
}

void StaticSprite::update(float dt)
{
	updateAABB();
	move(velocity.x*dt, velocity.y*dt);
}