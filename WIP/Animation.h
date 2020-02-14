#pragma once
#include "SFML\Graphics.hpp"
class Animation
{
public:
	Animation();
	~Animation();
	void init(int left, int top, int width, int height, int frames, float speed, bool loop);
	sf::IntRect getCurrentFrame();
	float getAnimationTime() { return animationSpeed; };
	void nextFrame();
	bool flipped;
	int currentFrame;
	bool keepLooping;
	bool looping;
	void flip();
	// could add functionality for play, pause, stop and loop
protected:
	sf::IntRect frame;
	int numberOfFrames;
	float animationSpeed;
};