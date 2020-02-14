#include "Predictor.h"

Predictor::Predictor()
{
}


Predictor::~Predictor()
{
}

bool Predictor::canExtraPolate()
{
	// Stop out of range boundaries.
	if (messages.size() > 3)
	{
		// Check if the player is moving, if they are then we can predict otherwise they are just the same as before.
		if (messages[2].velo.x != 0)
		{
			// Do i have enough messages to predict? 3?
			return (messages.size() == 3);
		}
		else
		{
			return false;
		}
	}
	return false;
}

float Predictor::ExtrapolateLinear(float currentTime)
{
	// (velocity * time) + distance = new distance.
	return (messages[2].velo.x * currentTime) + messages[2].info;
	/*return (messgaes)*/
}

float Predictor::ExtrapolateQuadratic(float currentTime)
{
	return (pow(messages[2].info, 2) * currentTime) + (messages[1].info*currentTime) + messages[0].info;
}

float Predictor::LinearInterpolate(float currentTime)
{
	// Lerp Function, to lerp more towards
	return messages[1].info * (1.0 - currentTime) * currentTime + messages[2].info;
}

void Predictor::AddMessage(GamePacket* packet)
{
	if (messages.size() != 3)
	{
		messages.push_back(*packet);
	}
	else
	{
		if (!messages.empty())
		{
			messages.erase(messages.begin());
		}
		messages.push_back(*packet);
	}
}