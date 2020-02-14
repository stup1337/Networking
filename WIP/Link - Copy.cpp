#include "Link.h"




Link::Link(const sf::Vector2f & size) : AnimatedSprite(size)
{
	
	walkDown.init(0, 0, 32, 48, 4, 0.40f);
	walkLeft.init(0, 96, 31, 48, 4, 0.10f);
	walkUp.init(0, 144, 32, 48, 4, 0.40f);
	currentAnimation = &walkDown;
	frame = currentAnimation->getCurrentFrame();
	setTextureRect(frame);
	speed = 50.f;
	acceleration = 20.f;
	target = sf::Vector2f(600, 300);
	moving = true;
	scale = 200.0;
	
}

Link::~Link()
{
}

void Link::update(float dt)
{
	move(velocity.x*dt, velocity.y*dt);

	if (animating)
	{
		elapsedTime += dt;
		if (elapsedTime >= currentAnimation->getAnimationTime())
		{
			currentAnimation->nextFrame();
			setTextureRect(currentAnimation->getCurrentFrame());
			elapsedTime = 0;
		}
	}

	
	
}

void Link::takeInput(Input* input)
{
	if (input->isKeyDown(sf::Keyboard::D))
	{
		input->setKeyUp(sf::Keyboard::D);
		setPosition(getPosition().x + 16, getPosition().y);
		if (currentAnimation->flipped)
		{
			currentAnimation->flip();
		}
		currentAnimation = &walkLeft;
		
	}
	if (input->isKeyDown(sf::Keyboard::A))
	{
		input->setKeyUp(sf::Keyboard::A);
		setPosition(getPosition().x - 16, getPosition().y); 
		currentAnimation->flipped = true;
		currentAnimation = &walkLeft;
	}
	if (input->isKeyDown(sf::Keyboard::S))
	{
		input->setKeyUp(sf::Keyboard::S);
		setPosition(getPosition().x, getPosition().y + 16);
		currentAnimation = &walkDown;
	}

	if (input->isKeyDown(sf::Keyboard::W))
	{
		input->setKeyUp(sf::Keyboard::W);
		setPosition(getPosition().x, getPosition().y - 16);
		currentAnimation = &walkUp;
	}
}

void Link::flipSprite()
{
	currentAnimation->flip();
}

