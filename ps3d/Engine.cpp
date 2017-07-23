#include "Engine.h"
#include "GameReport.h"
#include <iostream>

static const int SCREEN_WIDTH = 1680;
static const int SCREEN_HEIGHT = 1050;

static const bool IS_FULLSCREEN = false;

static const float DEFAULT_MOVE_SPEED = 3.0f;
static const float DEFAULT_ROTATE_SPEED = 4.0f;

extern const float COLLISION_RADIUS = 0.33f;

void ps3d::Engine::tick()
{
	sf::Event event;
	while (window->pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window->close();
	}

	//speed modifiers
	float moveSpeed = float(frameTime) * moveSpeedConst;
	float rotateSpeed = float(frameTime) * rotateSpeedConst;

	//movement
	bool forward = sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W);
	bool back = sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S);
	bool left = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
	bool right = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
	bool keyPressed = forward || back || left || right;
	if (keyPressed)
	{
		sf::Vector2f newPos(player->x, player->y);
		if (forward)
		{
			newPos.x += player->getDirX() * moveSpeed;
			newPos.y += player->getDirY() * moveSpeed;
		}
		if (back)
		{
			newPos.x -= player->getDirX() * moveSpeed;
			newPos.y -= player->getDirY() * moveSpeed;
		}
		if (right)
		{
			newPos.x += player->getDirY() * moveSpeed;
			newPos.y += -player->getDirX() * moveSpeed;
		}
		if (left)
		{
			newPos.x += -player->getDirY() * moveSpeed;
			newPos.y += player->getDirX() * moveSpeed;
		}
		// collision
		sf::Vector2<bool> canMove = map->canMove(newPos, sf::Vector2f(player->x, player->y));
		if (canMove.x)
		{
			player->x = newPos.x;
		}
		if (canMove.y)
		{
			player->y = newPos.y;
		}
		//std::cout << moveSpeed << std::endl;
	}
	//rotate to the right
	float rotateRad = 0;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		rotateRad = -rotateSpeed;
	}
	//rotate to the left
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		rotateRad = rotateSpeed;
	}
	if (window->hasFocus())
	{
		int windowMid = window->getSize().x / 2;
		int mousePos = sf::Mouse::getPosition(*window).x;
		if (mousePos != windowMid)
		{
			rotateRad = (windowMid - mousePos) / 1000.0f;
			sf::Mouse::setPosition(sf::Vector2i(windowMid, window->getSize().y / 2), *window);
		}
	}
	if (rotateRad != 0)
	{
		player->rotate(rotateRad);
	}
	curGameReport = game->tick(frameTime);
	map->tick(frameTime);
	player->tick(frameTime);

	float fps = 1.0f / float(frameTime);
	renderer->render(curGameReport, int(fps));
	frameTime = clock->restart().asSeconds();
}

ps3d::Engine::Engine(ps3d::IGame* game)
{
	this->window = nullptr;
	this->clock = new sf::Clock;
	this->game = game;
	this->frameTime = 0;
	this->moveSpeedConst = DEFAULT_MOVE_SPEED;
	this->rotateSpeedConst = DEFAULT_ROTATE_SPEED;
	this->player = game->getPlayer();
	this->map = game->getMap();
	this->miniMap = game->getMiniMap();
	this->renderer = nullptr;
}

bool ps3d::Engine::isCollision(Sprite* sprite, sf::Vector2f coords)
{
	return isCollision(sf::Vector2f(sprite->x, sprite->y), coords);
}

bool ps3d::Engine::isCollision(sf::Vector2f coords1, sf::Vector2f coords2)
{
	//x
	sf::Vector2f coords1Square[2];
	coords1Square[0] = sf::Vector2f(coords1.x - COLLISION_RADIUS, coords1.y - COLLISION_RADIUS);
	coords1Square[1] = sf::Vector2f(coords1.x + COLLISION_RADIUS, coords1.y + COLLISION_RADIUS);
	return coords2.x >= coords1Square[0].x
		&& coords2.x <= coords1Square[1].x
		&& coords2.y >= coords1Square[0].y
		&& coords2.y <= coords1Square[1].y;
}

void ps3d::Engine::setMoveSpeed(float moveSpeed = DEFAULT_MOVE_SPEED)
{
	moveSpeedConst = moveSpeed;
}

void ps3d::Engine::setRotateSpeed(float rotateSpeed = DEFAULT_ROTATE_SPEED)
{
	rotateSpeedConst = rotateSpeed;
}

void ps3d::Engine::start()
{
	int style = IS_FULLSCREEN ? sf::Style::Fullscreen : 0;
	window = new sf::RenderWindow(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), game->getName(), style);
	window->setVerticalSyncEnabled(true);
	window->setMouseCursorVisible(false);
	renderer = new Renderer(window, map, player, miniMap);

	game->start();
	while (window->isOpen() && !curGameReport.isEnd)
	{
		tick();
	}
}