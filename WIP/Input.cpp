#include "Input.h"


void Input::setKeyDown(int key)
{
	keys[key] = true;
}

void Input::setKeyUp(int key)
{
	keys[key] = false;
}

bool Input::isKeyDown(int key)
{
	return keys[key];
}

void Input::setMouseX(int lx)
{
	mouse.x = lx;
}

void Input::setMouseY(int ly)
{
	mouse.y = ly;
}

void Input::setMousePosition(int lx, int ly)
{
	mouse.x = lx;
	mouse.y = ly;
}

int Input::getMouseX()
{
	return mouse.x;
}

int Input::getMouseY()
{
	return mouse.y;
}

bool Input::getMouseLeftDown()
{
	return mouse.left;
}
bool Input::getMouseRightDown()
{
	return mouse.right;
}

void Input::setMouseLeftDown(bool is)
{
	mouse.left = is;
}

void Input::setMouseRightDown(bool is)
{
	mouse.right = is;
}

bool Input::isTherePlayerInputWSAD()
{
	return (keys[3] | keys[18] || keys[22] || keys[0]); // Is there any movement with the W,S,A,D keys.
}

bool Input::isTherePlayerInputQE()
{
	return (keys[16] | keys[4]);
}

Input::Input()
{
}


Input::~Input()
{
}
