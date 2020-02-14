#include "Animation.h"
Animation::Animation()
{
	currentFrame = 0;
}
Animation::~Animation()
{}
void Animation::init(int left, int top, int width, int height, int numFrames, float speed, bool loop)
{
	frame.left = left;
	frame.top = top;
	frame.width = width;
	frame.height = height;
	numberOfFrames = numFrames;
	animationSpeed = speed;
	looping = loop;
	keepLooping = loop;
	
}
sf::IntRect Animation::getCurrentFrame()
{
	sf::IntRect temp;
	if (!flipped)
	{
		temp = sf::IntRect(frame.width * currentFrame, frame.top, frame.width, frame.height);
	}
	else
		temp = sf::IntRect(frame.width  * (currentFrame + 1), frame.top, -frame.width, frame.height);

	return temp;
}

void Animation::nextFrame()
{
	if (currentFrame < numberOfFrames)
	{
		currentFrame++;
	}
		if (currentFrame >= numberOfFrames)
		{
			if (looping)
			{
				currentFrame = 0;
			}
			else 
			{
			currentFrame = numberOfFrames - 1;
			}
		}
}

void Animation::flip()
{
	if (flipped)
	{
		flipped = false;
	}
	else
	{
		flipped = true;
	}
}