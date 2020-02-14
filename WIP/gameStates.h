#pragma once
enum class GameState { MENU, LEVEL, PAUSE, DEATH, TUTORIAL,UPGRADE};
enum MovementState { IDLE, WALK, ATTACK, DEATH, BLOCK};
enum MovingDirection { LEFT, RIGHT };
struct PlayerPacket
{
	MovementState state;
	MovingDirection direction;
};

struct PosPacket
{
	float x;
};