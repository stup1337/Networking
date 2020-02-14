#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "Input.h"
#include <windows.h>
#include <string.h>
#include "AnimatedSprite.h"
#include "Player.h"
#include "StaticSprite.h"
#include "gameStates.h"
#include "AudioManager.h"
#include "Boss.h"
#include "Protocol.h"
#include <iostream>
#include "Predictor.h"



class Game {
public:
	Game(sf::RenderWindow* hwnd, Input* input);
	~Game();
	// Set up functions
	void ServerSetUp();
	void ClientSetUp();
	// Server and Client packet sending
	void Server_SendPositionPacket(int FromplayerType);
	void Server_SendDirectionalPacket(int FromplayerType);
	void Server_SendMovementStatePacket(int fromplayerType);
	void Server_SendSyncPacket();
	int Server_SendToWho(int fromPlayerType);
	// Packet Sending for clients
	void Client_SendPositionPacket();
	void Client_SendDirectionPacket();
	void Client_SendMovementStatePacket();
	// Game Functions
	void handleInput(float dt, Input* in);
	void update(float dt);
	void render();
	void Reset();
	bool checkSwordPlayerCollision(Player * s1, sf::RectangleShape * s2);
	bool checkSwordShieldCollision(sf::RectangleShape * s1, sf::RectangleShape * s2);
	// Sending and receiving casting
	MovingDirection IntrepretDirection(int direction);
	MovementState IntrepretMovement(int movementState);
	sf::Uint8 IntrepretIntToDirection(MovingDirection movingDirection);
	sf::Uint8 IntrepretIntToMovement(MovementState movementState);
	// Determining if the application is the server function
	void Receive(bool isServer);
	float timer = 0.0f;
	bool G_isServer = false;
	// Game assets
	sf::Sprite bg;
	sf::Texture playerText;
	sf::Texture player2Text;
	sf::Texture bgText;
	sf::Texture bossText;
	sf::Texture bombText;
	// Players
	Player Players[2];
	// Networking Utilites and timers
	sf::SocketSelector selector;	
	unsigned short SERVERPORT = 4444;
	bool player2 = false;	// Client Player Identifier
	sf::IpAddress SERVERIP = ("127.0.0.1");
	std::vector<Client> clients;
	sf::UdpSocket socket;
	int playerCounts;
	enum ServerState { LOOKING, GAME };
	ServerState servState;
	float serverTimer;
	float clientTimer;
	// SERVER TIME
	float serverTime;
	float syncTimer;
	// Prediction utility
	Predictor predictor;
private:
	sf::RenderWindow* window;
	Input* input;
	void beginDraw();
	void endDraw();
};