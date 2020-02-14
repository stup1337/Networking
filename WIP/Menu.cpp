#include "Menu.h"
#include "Input.h"


Menu::Menu(sf::RenderWindow* hwnd, Input* in)
{
	window = hwnd;
	input = in;
	input->setMouseLeftDown(false);

	bgText.loadFromFile("Images/bg.png");
	bg.setTexture(bgText);
	//bg.setSize(sf::Vector2f(800, 600));
	bg.setPosition(sf::Vector2f(0, -270));
	
}
Menu::~Menu()
{
}

void Menu::update(float dt)
{

}
void Menu::handleInput(float dt, Input* in)
{
	if (input->isKeyDown(sf::Keyboard::S))
	{
		isServer = true;
		madeDecision = true;
	}
	if (input->isKeyDown(sf::Keyboard::C))
	{
		isServer = false;
		madeDecision = true;
	}
}

void Menu::render()
{
	beginDraw();
	window->setMouseCursorVisible(false);
	endDraw();
}
void Menu::beginDraw()
{
	window->clear(sf::Color::Black);
	window->draw(bg);
}
void Menu::endDraw()
{
	window->display();
}

Decision Menu::CheckDecision()
{
	Decision temp;
	temp.decisionMade = false;
	temp.isServer = false;
	if (madeDecision)
	{
		temp.decisionMade = true;
		temp.isServer = isServer;
	}
	
	return temp;
}
