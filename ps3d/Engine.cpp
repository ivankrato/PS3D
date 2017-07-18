#include "Engine.h"
#include <iostream>

static const int SCREEN_WIDTH = 1280;
static const int SCREEN_HEIGHT = 720;

static const float DEFAULT_MOVE_SPEED = 3.0f;
static const float DEFAULT_ROTATE_SPEED = 5.0f;

void ps3d::Engine::tick()
{
	sf::Event event;
	while (window->pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window->close();
	}

	//speed modifiers
	float moveSpeed = frameTime * moveSpeedConst;
	float rotateSpeed = frameTime * rotateSpeedConst;

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
		bool canMoveX = !map->willCollideX(newPos.x, player->y);
		bool canMoveY = !map->willCollideY(newPos.y, player->x);
		// TODO sprite collision
		/*for (Sprite& sprite : sprites)
		{
		sf::Vector2f spriteSquare[2];
		spriteSquare[0] = sf::Vector2f(sprite.x - block, sprite.y - block);
		spriteSquare[1] = sf::Vector2f(sprite.x + block, sprite.y + block);
		if (newPosX >= spriteSquare[0].x
		&& newPosX <= spriteSquare[1].x
		&& posY >= spriteSquare[0].y
		&& posY <= spriteSquare[1].y) {
		canMoveX = false;
		}
		if (newPosY >= spriteSquare[0].y
		&& newPosY <= spriteSquare[1].y
		&& posX >= spriteSquare[0].x
		&& posX <= spriteSquare[1].x) {
		canMoveY = false;
		}
		}*/
		if (canMoveX)
		{
			player->x = newPos.x;
		}
		if (canMoveY)
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
	if(window->hasFocus())
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
	game->tick();
	renderer->render();

	frameTime = clock->restart().asSeconds();
	//float fps = 1.0 / frameTime;
}

ps3d::Engine::Engine(ps3d::IGame* game)
{
	this->window = new sf::RenderWindow(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), game->getName());
	this->clock = new sf::Clock;
	this->game = game;
	this->frameTime = 0;
	this->moveSpeedConst = DEFAULT_MOVE_SPEED;
	this->rotateSpeedConst = DEFAULT_ROTATE_SPEED;
	this->player = game->getPlayer();
	this->map = game->getMap();
	this->renderer = new Renderer(window, map, player);
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
	game->start();
	while (window->isOpen())
	{
		tick();
	}
}