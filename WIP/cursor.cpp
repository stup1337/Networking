#include "cursor.h"



cursor::cursor(const sf::Vector2f & size) : Sprite(size)
{

}


cursor::~cursor()
{
}

void cursor::update(float dt)
{
}
void cursor::takeInput(Input* input)
{
	setPosition(input->getMouseX(), input->getMouseY());
}