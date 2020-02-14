#pragma once
#include "SFML\Main.hpp"
#include "SFML\Network.hpp"
// Player Slot -> 0-1
// Player Position, Player Direction, Player State, Timing , 0-3
// DATA

// MSGTYPE = 0,1,2,3,4 = Player Position Updates,Direction Updates, State Updates, Timing updates.

struct GamePacket
{
	sf::Uint8 msgType;
	sf::Uint8 playerType;
	float time;
	float info;
	sf::Vector2f velo;
	sf::Uint8 enumInfo;
};

struct PlayerTypePacket 
{
	sf::Uint8 playertype;
};

struct IntialConnectionPacket
{
	unsigned short port;
	sf::IpAddress IP;
};

struct PlayerIDPacket
{
	bool ID;
	float serverTime;
};

struct Client
{
	Client(sf::IpAddress p_IP, unsigned short p_port)
	{
		IP = p_IP;
		port = p_port;
	}
	sf::IpAddress IP;
	unsigned short port;
};
