#include "Game.h"
#include "Input.h"



Game::Game(sf::RenderWindow* hwnd, Input* in)
{
	window = hwnd;
	input = in;
	input->setMouseLeftDown(false);
	serverTimer = 0;
	clientTimer = 0;
	playerCounts = 0;
	playerText.loadFromFile("Images/mc.png");
	player2Text.loadFromFile("Images/mc.png");
	

	Players[0].setFrameSize(32, 384);
	Players[0].setSize(sf::Vector2f(256, 256));
	Players[0].setTexture(&playerText);
	Players[0].setAnimationSpeed(0.1);
	Players[0].animating = true;
	Players[0].setAlive(true);
	Players[0].setPosition(100, 450);
	Players[0].setFillColor(sf::Color(255, 160, 160, 250));

	Players[1].setFrameSize(32, 384);
	Players[1].setSize(sf::Vector2f(256, 256));
	Players[1].setTexture(&player2Text);
	Players[1].setAnimationSpeed(0.1);
	Players[1].animating = true;
	Players[1].setAlive(true);
	Players[1].SetPlayer2(true); // Sets the Position to the Player2's stuff.
	Players[1].setFillColor(sf::Color(160, 160, 255, 250));
	//playerCount = 0;
	bgText.loadFromFile("Images/bg.png");
	//bg.setSize(sf::Vector2f(800, 600));
	bg.setTexture(bgText);
	bg.setPosition(sf::Vector2f(0, -270));
	
	
}
Game::~Game()
{
}

void Game::ServerSetUp()
{
	Client tempClient(sf::IpAddress::None, 3500);
	clients.push_back(tempClient);
	socket.setBlocking(true);
	socket.bind(SERVERPORT, SERVERIP);
	selector.add(socket);
	window->setTitle("Server");
	std::cout << "Server Set up!" << std::endl;
	servState = LOOKING;
	serverTime = 0.0f;
	syncTimer = 0.0f;
}

void Game::ClientSetUp()
{
	Client tempClient(sf::IpAddress::getLocalAddress(), 1234);
	clients.push_back(tempClient);
	socket.setBlocking(true);
	socket.bind(socket.AnyPort, SERVERIP);
	selector.add(socket);
	std::cout << "Binded to port" << socket.getLocalPort() << std::endl;
	servState = LOOKING;
	window->setTitle("Client");
	serverTime = 0.0f;
	syncTimer = 0.0f;
}
void Game::update(float dt)
{
	switch (servState)
	{
	case LOOKING:
	{
		// I am the server looking for players, receive packets.
		if (G_isServer)
		{
			serverTime += dt;
			IntialConnectionPacket packet;
			sf::Packet receivedP;
			//std::cout << "Awaiting new clients \n";
			socket.receive(receivedP, packet.IP, packet.port);
			//clients.resize(1)z

			
			if (clients[playerCounts].port != packet.port && packet.port != NULL)
			{
				if (clients.size() < 4)	// There are 3 clients in the list, we need 3 as we needed to put one inside the vector to do a quick check.
				{
					clients.push_back(Client(packet.IP, packet.port));
					std::cout << "Added new Client \n ";
					std::cout << packet.IP << " " << packet.port << "\n";
					playerCounts++;
				}
			}

			if (clients.size() == 3)
			{
				for (int i = 0; i < clients.size(); i++)
				{
					std::cout << clients[i].IP << " " << clients[i].port << std::endl;
				}
				clients.erase(clients.begin());
				std::cout << "Players found! Starting Game";

				for (int i = 0; i < 2; i++)
				{
						PlayerIDPacket info;
						sf::Packet packet;
						info.ID = i;
						packet << info.ID;
						packet << serverTime;
						socket.send(packet, clients[i].IP, clients[i].port);
						std::cout << "Sent Packet to" << clients[i].IP << " " << clients[i].port;
				}
				
				servState = GAME;
			}

			/*for (int i = 0; i < clients.size(); i++)
			{
				std::cout << clients[i].IP << " " << clients[i].port << std::endl;
			}*/

		}
		else
		{
			// Else I am the client, bind to the ServerIP, send my information to let the server register me as a client and once that is done, start the game.
			IntialConnectionPacket packet;
			packet.IP = clients[0].IP;
			packet.port = clients[0].port;
			sf::Packet sendingPacket;
			sendingPacket << packet.IP.toString() << packet.port;
			socket.send(sendingPacket, SERVERIP, SERVERPORT);

			if (selector.wait(sf::seconds(10.0f)))
			{
				if (selector.isReady(socket))
				{
					PlayerIDPacket info;
					sf::Packet packet;
					socket.receive(packet, SERVERIP, SERVERPORT);
					packet >> info.ID;
					packet >> serverTime;
					if (info.ID == 1 || info.ID == 0)
					{
						player2 = info.ID;
					}
				}
			}
			if (player2 == 0 || 1 && serverTime > 0)
			{
				std::cout << "Socket has been connected";
				servState = GAME;
			}
		}
		break;
	}
	case GAME:
	{
		if (G_isServer)
		{
			serverTime += dt;
			float oldPlayerPos1 = Players[0].getPosition().x;
			float oldPlayerPos2 = Players[1].getPosition().x;
			
			MovingDirection oldMovDirect1 = Players[0].playerDirection;
			MovingDirection oldMovDirect2 = Players[1].playerDirection;

			MovementState oldPlayerMovement1 = Players[0].playerMovement;
			MovementState oldPlayerMovement2 = Players[1].playerMovement;
			serverTimer += dt;
			syncTimer += dt;

			if (syncTimer > 10.0f)
			{
				std::cout << "Server time : " << serverTime;
				Server_SendSyncPacket();
				syncTimer = 0;
			}
			if (serverTimer > 0.020)
			{
				serverTimer = 0;
				if (selector.wait(sf::milliseconds(1)))
				{
					if (selector.isReady(socket))
					{
						sf::Packet tempPacket;
						GamePacket tempGamePacket;
						socket.receive(tempPacket, SERVERIP, SERVERPORT);
						tempPacket >> tempGamePacket.msgType >> tempGamePacket.playerType >> tempGamePacket.time >> tempGamePacket.info >> tempGamePacket.enumInfo >> tempGamePacket.velo.x >> tempGamePacket.velo.y;

						if (tempGamePacket.msgType == 0)
						{
							Players[tempGamePacket.playerType].setPosition(tempGamePacket.info, Players[0].getPosition().y);
						}
						if (tempGamePacket.msgType == 1)
						{
							Players[tempGamePacket.playerType].playerDirection = IntrepretDirection(tempGamePacket.enumInfo);
						}

						if (tempGamePacket.msgType == 2)
						{
							Players[tempGamePacket.playerType].playerMovement = IntrepretMovement(tempGamePacket.enumInfo);
						}
						
					}
				}
				// Check if receiving packets have affected the Players, send new packets to other players if not in sync

				if (oldPlayerPos1 != Players[0].getPosition().x)
				{
					Server_SendPositionPacket(0);
				}
				if (oldPlayerMovement1 != Players[0].playerMovement)
				{
					Server_SendDirectionalPacket(0);
				}

				if (oldMovDirect1 != Players[0].playerDirection)
				{
					Server_SendMovementStatePacket(0);
				}

				if (oldPlayerPos2 != Players[1].getPosition().x)
				{
					Server_SendPositionPacket(1);
				}
				if (oldPlayerMovement2 != Players[1].playerMovement)
				{
					Server_SendDirectionalPacket(1);
				}

				if (oldMovDirect2 != Players[1].playerDirection)
				{
					Server_SendMovementStatePacket(1);
				}
			}
			timer += dt;
			Players[0].update(dt);
			Players[1].update(dt);
		}
		else
		{
			// CLIENT AREA OF THE PROGRAM
			// Keeping track of changes
			// Receiving Packet
			serverTime += dt;
			//std::cout << "Server time : " << serverTime;
			//std::cout << "Player  : " << player2 << " Pos : " << Players[player2].getPosition().x << " \n";
			clientTimer += dt;

			if (clientTimer > 0.020)
			{
				// not resetting as I tie this timer in with the server, it should only be receiving as much as the server is sending, this timer will get changed to 0 later anyways.
				if (selector.wait(sf::milliseconds(1)))
				{
					if (selector.isReady(socket))
					{
						sf::Packet recevingPacket;
						GamePacket gameReceivingPacket;
						socket.receive(recevingPacket, SERVERIP, clients[0].port);
						recevingPacket >> gameReceivingPacket.msgType >> gameReceivingPacket.playerType >> gameReceivingPacket.time >> gameReceivingPacket.info >> gameReceivingPacket.enumInfo >> gameReceivingPacket.velo.x >> gameReceivingPacket.velo.y;
						// The message is a position update. Update the player involved in the packet.
						if (gameReceivingPacket.msgType == 0)
						{
							// add the lastest message
							predictor.AddMessage(&gameReceivingPacket);
							// Check if the previous prediction was super wrong.
							if (predictor.canExtraPolate())	// Is just a checking function for the size as well as checking if velocity == 0
							{
								if (abs(gameReceivingPacket.info) - abs(predictor.messages[1].info) > 5)
								{
									gameReceivingPacket.info = predictor.LinearInterpolate((serverTime - gameReceivingPacket.time) / 2);
								}
							}
							Players[gameReceivingPacket.playerType].setPosition(gameReceivingPacket.info, Players[0].getPosition().y);
							if (predictor.canExtraPolate())
							{
								gameReceivingPacket.info = predictor.ExtrapolateLinear((serverTime - gameReceivingPacket.time) / 2);
								Players[gameReceivingPacket.playerType].setPosition(gameReceivingPacket.info, Players[0].getPosition().y);
							}
							std::cout << "Player Position Recevied" << std::endl;
						}
						// The message is a Direction update. Update the player involved in the packet
						if (gameReceivingPacket.msgType == 1)
						{
							Players[gameReceivingPacket.playerType].playerDirection = IntrepretDirection(gameReceivingPacket.enumInfo);
							std::cout << gameReceivingPacket.enumInfo << "Player Direction received. \n";
						}
						// The message is a State Update. Update the player involved in the packet
						if (gameReceivingPacket.msgType == 2)
						{
							Players[gameReceivingPacket.playerType].playerMovement = IntrepretMovement(gameReceivingPacket.enumInfo);
							std::cout << gameReceivingPacket.enumInfo << "Player State received. \n";
						}

						if (gameReceivingPacket.msgType == 4)
						{
							// If the message is a time update, I'll set the time to this plus adding on any latency.
							serverTime = gameReceivingPacket.time - ((serverTime - gameReceivingPacket.time)/2);
							std::cout << "Server time : " << serverTime;
						}
					}
				}
			}
			
			// Save old information for use later on, We'll use this to determine if we need to do a client send update.
			float oldPlayerPos = Players[player2].getPosition().x;
			MovingDirection oldMovDirect = Players[player2].playerDirection;
			MovementState oldPlayerMovement = Players[player2].playerMovement;
			
			// CLIENT UPDATE
			timer += dt;
			Players[0].update(dt);
			Players[1].update(dt);

			// Send up to 50 times per second.
			if (clientTimer > 0.020)
			{
				clientTimer = 0;
				// Check for changes, send packets accordingly.
				if (oldPlayerPos != Players[player2].getPosition().x)
				{
					Client_SendPositionPacket();
				}
				if (oldMovDirect != Players[player2].playerDirection)
				{
					Client_SendDirectionPacket();
				}

				if (oldPlayerMovement != Players[player2].playerMovement)
				{
					Client_SendMovementStatePacket();
				}
			}

		}

		break;
	}
	}
}

// Client packet functions
void Game::Client_SendPositionPacket()
{
	// Sending Packet about Position
	sf::Packet sendingPacket;
	GamePacket gamePacket;
	gamePacket.msgType = 0;
	gamePacket.playerType = player2;
	gamePacket.time = serverTime;
	gamePacket.enumInfo = 0;
	gamePacket.info = Players[player2].getPosition().x;
	gamePacket.velo = Players[player2].getVelocity();
	sendingPacket << gamePacket.msgType << gamePacket.playerType << gamePacket.time << gamePacket.info << gamePacket.enumInfo << gamePacket.velo.x << gamePacket.velo.y;

	socket.send(sendingPacket, SERVERIP, SERVERPORT);
	//Network Update
}

void Game::Client_SendDirectionPacket()
{
	// Sending Packet about Direction
	sf::Packet sendingPacket;
	GamePacket gamePacket;
	gamePacket.msgType = 1;
	gamePacket.playerType = player2;
	gamePacket.info = 0;
	gamePacket.time = serverTime;
	gamePacket.velo = sf::Vector2f(0,0);
	gamePacket.enumInfo = IntrepretIntToDirection(Players[player2].playerDirection);

	sendingPacket << gamePacket.msgType << gamePacket.playerType << gamePacket.time << gamePacket.info << gamePacket.enumInfo << gamePacket.velo.x << gamePacket.velo.y;

	socket.send(sendingPacket, SERVERIP, SERVERPORT);
	//Network Update
}

void Game::Client_SendMovementStatePacket()
{
	// Sending Packet about Movement
	sf::Packet sendingPacket;
	GamePacket gamePacket;
	gamePacket.msgType = 2;
	gamePacket.playerType = player2;
	gamePacket.info = 0;
	gamePacket.time = serverTime;
	gamePacket.velo = sf::Vector2f(0, 0);
	gamePacket.enumInfo = IntrepretIntToMovement(Players[player2].playerMovement);

	sendingPacket << gamePacket.msgType << gamePacket.playerType << gamePacket.time << gamePacket.info << gamePacket.enumInfo << gamePacket.velo.x << gamePacket.velo.y;

	socket.send(sendingPacket, SERVERIP, SERVERPORT);
}
// Client Packet functions end
// Server Packet Functions 

// Send a Positional packet of a specific player's info for the other player.
void Game::Server_SendPositionPacket(int FromplayerType)
{
	int sendTo = Server_SendToWho(FromplayerType);
	sf::Packet sendingPacket;
	GamePacket gamePacket;
	gamePacket.msgType = 0;
	gamePacket.playerType = FromplayerType;
	gamePacket.time = serverTime;
	gamePacket.enumInfo = 0;
	gamePacket.velo = Players[FromplayerType].getVelocity();
	gamePacket.info = Players[FromplayerType].getPosition().x;
	sendingPacket << gamePacket.msgType << gamePacket.playerType << gamePacket.time << gamePacket.info << gamePacket.enumInfo << gamePacket.velo.x << gamePacket.velo.y;

	socket.send(sendingPacket, clients[sendTo].IP, clients[sendTo].port);
}
// Send a packet containing directional data, which is from a specific player to be applied on another clients screen.
void Game::Server_SendDirectionalPacket(int FromplayerType)
{
	int sendTo = Server_SendToWho(FromplayerType);
	sf::Packet sendingPacket;
	GamePacket gamePacket;
	gamePacket.msgType = 1;
	gamePacket.playerType = FromplayerType;
	gamePacket.time = serverTime;
	gamePacket.enumInfo = IntrepretIntToDirection(Players[FromplayerType].playerDirection);
	gamePacket.velo = sf::Vector2f(0, 0);
	gamePacket.info = 0;
	sendingPacket << gamePacket.msgType << gamePacket.playerType << gamePacket.time << gamePacket.info << gamePacket.enumInfo << gamePacket.velo.x << gamePacket.velo.y;

	socket.send(sendingPacket, clients[sendTo].IP, clients[sendTo].port);
}
// Send a Movement State Packet.
void Game::Server_SendMovementStatePacket(int FromplayerType)
{
	int sendTo = Server_SendToWho(FromplayerType);
	sf::Packet sendingPacket;
	GamePacket gamePacket;
	gamePacket.msgType = 2;
	gamePacket.playerType = FromplayerType;
	gamePacket.time = serverTime;
	gamePacket.enumInfo = IntrepretIntToMovement(Players[FromplayerType].playerMovement);
	gamePacket.velo = sf::Vector2f(0, 0);
	gamePacket.info = 0;
	sendingPacket << gamePacket.msgType << gamePacket.playerType << gamePacket.time << gamePacket.info << gamePacket.enumInfo << gamePacket.velo.x << gamePacket.velo.y;

	socket.send(sendingPacket, clients[sendTo].IP, clients[sendTo].port);
}
// Send a sync time packet.
void Game::Server_SendSyncPacket()
{
	sf::Packet sendingPacket;
	GamePacket gamePacket;
	gamePacket.msgType = 4;
	gamePacket.time = serverTime;
	gamePacket.enumInfo = 0;
	gamePacket.velo = sf::Vector2f(0, 0);
	gamePacket.info = 0;
	sendingPacket << gamePacket.msgType << gamePacket.playerType << gamePacket.time << gamePacket.info << gamePacket.enumInfo << gamePacket.velo.x << gamePacket.velo.y;

	socket.send(sendingPacket, clients[0].IP, clients[0].port);
	socket.send(sendingPacket, clients[1].IP, clients[1].port);
}

int Game::Server_SendToWho(int fromPlayerType)
{
	if (fromPlayerType == 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
// Server Packet Functions end

void Game::handleInput(float dt, Input* in)
{
	if (!G_isServer)
	{
		Players[player2].takeInput(in);
		//Player2.takeInput(input);
	}

	if (input->isKeyDown(sf::Keyboard::L))
	{
		predictor.isEnabledLinear = !predictor.isEnabledLinear;
	}
}

void Game::render()
{
	beginDraw();
	window->setMouseCursorVisible(false);
	endDraw();
}
void Game::beginDraw()
{
	window->clear(sf::Color::Black);
	window->draw(bg);
	//window->draw(boss);
	for (int i = 0; i < 2; i++)
	{
		window->draw(Players[i].HealthBar);
		window->draw(Players[i].StaminaBar);
		//window->draw(boss.bomb);
		window->draw(Players[i].swordRect);
		window->draw(Players[i].shieldRect);
		window->draw(Players[i]);
	}
}
void Game::endDraw()
{
	window->display();
}
void Game::Reset()
{


}

bool Game::checkSwordPlayerCollision(Player* s1, sf::RectangleShape* s2)
{
	if (s1->getAABB().left + s1->getAABB().width < s2->getPosition().x)
		return false;
	if (s1->getAABB().left > s2->getPosition().x + s2->getSize().x)
		return false;
	if (s1->getAABB().top + s1->getAABB().height < s2->getPosition().y)
		return false;
	if (s1->getAABB().top > s2->getPosition().y + s2->getSize().y)
		return false;

	return true;
}

bool Game::checkSwordShieldCollision(sf::RectangleShape* s1, sf::RectangleShape* s2)
{
	if (s1->getPosition().x + s1->getSize().x < s2->getPosition().x)
		return false;
	if (s1->getPosition().x > s2->getPosition().x + s2->getSize().x)
		return false;
	if (s1->getPosition().y + s1->getSize().y < s2->getPosition().y)
		return false;
	if (s1->getPosition().y > s2->getPosition().y + s2->getSize().y)
		return false;

	return true;

}

// Functions act as casting, casting work for my implementation so here is the way to cast them.
MovingDirection Game::IntrepretDirection(int direction)
{
	if (direction == 0)
	{
		return MovingDirection::LEFT;
	}
	else
	{
		return MovingDirection::RIGHT;
	}
	return MovingDirection::LEFT;
}

MovementState Game::IntrepretMovement(int movementState)
{
	switch (movementState)
	{
	case 0:
		return MovementState::IDLE;
		break;
	case 1:
		return MovementState::WALK;
		break;
	case 2:
		return MovementState::ATTACK;
		break;
	case 3: 
		return MovementState::DEATH;
		break;
	case 4:
		return MovementState::BLOCK;
		break;
	}
}

sf::Uint8 Game::IntrepretIntToDirection(MovingDirection movingDirection)
{
	if (movingDirection == MovingDirection::LEFT)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

sf::Uint8 Game::IntrepretIntToMovement(MovementState movementState)
{
	switch (movementState)
	{
	case MovementState::IDLE:
	{
		return 0;
		break;
	}
	case MovementState::WALK:
	{
		return 1;
		break;
	}
	case MovementState::ATTACK:
	{
		return 2;
		break;
	}
	case MovementState::DEATH:
	{
		return 3;
		break;
	}
	case MovementState::BLOCK:
	{
		return 4;
		break;
	}
	}
}

void Game::Receive(bool isServer)
{
	G_isServer = isServer;
	if (G_isServer)
	{
		ServerSetUp();
	}
	else
	{
		ClientSetUp();
	}
	return;
}


