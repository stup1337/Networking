#pragma once
#include <vector>
#include "Protocol.h"

class Predictor
{
public:
	Predictor();
	~Predictor();
	bool canExtraPolate();
	float ExtrapolateQuadratic(float currentTime);
	float ExtrapolateLinear(float currentTime);
	float LinearInterpolate(float currentTime);
	bool fixedResult = false;
	bool isEnabledLinear = false;
	void AddMessage(GamePacket* packet);
	std::vector<GamePacket> messages;
};

