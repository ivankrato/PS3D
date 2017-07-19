#include "Maze3D.h"
#include "../Walls.h"
#include "../Sprites.h"

Maze3D::Game::Game() : IGame()
{
	frameTime = 0;
	textures.clear();
	// load textures
	textures["walls"] = new ps3d::Texture("Maze3D/Resources/walls.png", false, 512, 512);
	textures["skybox"] = new ps3d::Texture("Maze3D/Resources/skybox.jpg", true);
	textures["spriteBarrel"] = new ps3d::Texture("Maze3D/Resources/sprite_barrel.png", false, 512, 512);
	// TODO random?
	map = new ps3d::Map(sf::Vector2i(30, 30), &player, textures["skybox"]);
	int mapWidth = map->getSize().x;
	int mapHeight = map->getSize().y;
	for (int i = 0; i < mapWidth; i++)
	{
		if (i == 0 || i == mapWidth - 1)
		{
			for (int j = 0; j < mapHeight; j++)
			{
				map->addWall(new ps3d::Wall(textures["walls"], 512), sf::Vector2i(i, j));
			}
		}
		else
		{
			if (i != 20)
				map->addWall(new ps3d::Wall(textures["walls"], 512), sf::Vector2i(i, 0));
			map->addWall(new ps3d::Wall(textures["walls"], 512), sf::Vector2i(i, mapHeight - 1));
		}
	}
	for (float i = 1.5; i < mapWidth - 1; i++)
	{
		ps3d::Sprite *sprite = new ps3d::Sprite(i, 15.5f, textures["spriteBarrel"], 0, true, false);
		ps3d::Map *map = this->map;
		sprite->setOnCollision([map, sprite]()
		{
			map->removeSprite(sprite);
			delete sprite;
		});
		map->addSprite(sprite);
	}

	map->addWall(new ps3d::Wall(textures["walls"]), sf::Vector2i(10, 1));
	map->addWall(new ps3d::Wall(textures["walls"]), sf::Vector2i(10, 2));
	map->addWall(new ps3d::Wall(textures["walls"]), sf::Vector2i(10, 3));
	map->addWall(new ps3d::Wall(textures["walls"]), sf::Vector2i(10, 4));
	map->addWall(new ps3d::Wall(textures["walls"]), sf::Vector2i(10, 5));
	map->addWall(new ps3d::Wall(textures["walls"]), sf::Vector2i(10, 6));
	map->addWall(new ps3d::Wall(textures["walls"]), sf::Vector2i(10, 7));
	map->addWall(new ps3d::Wall(textures["walls"]), sf::Vector2i(10, 8));
	map->addWall(new ps3d::Wall(textures["walls"]), sf::Vector2i(10, 9));
	map->addWall(new ps3d::Wall(textures["walls"]), sf::Vector2i(10, 10));

	map->addWall(new ps3d::Wall(textures["walls"]), sf::Vector2i(1, 10));
	map->addWall(new ps3d::Wall(textures["walls"]), sf::Vector2i(2, 10));
	map->addWall(new ps3d::Wall(textures["walls"]), sf::Vector2i(3, 10));
	map->addWall(new ps3d::Wall(textures["walls"]), sf::Vector2i(4, 10));
	map->addWall(new ps3d::Wall(textures["walls"]), sf::Vector2i(6, 10));
	map->addWall(new ps3d::Wall(textures["walls"]), sf::Vector2i(7, 10));
	map->addWall(new ps3d::Wall(textures["walls"]), sf::Vector2i(8, 10));
	map->addWall(new ps3d::Wall(textures["walls"]), sf::Vector2i(9, 10));
	map->addWall(new ps3d::Wall(textures["walls"]), sf::Vector2i(10, 10));

	ps3d::Sprite *sprite = new ps3d::Sprite(18.0f, 18.0f, textures["spriteBarrel"]);
	double *time = new double(0);
	double *frameTime = &this->frameTime;
	sprite->setOnTick([sprite, time](double frameTime)
	{
		if (*time > 1)
		{
			sprite->setNextOffset();
			sprite->collideable = sprite->offset < 512;
			*time = 0;
		}
		else
		{
			*time += frameTime;
		}
	});
	map->addSprite(sprite);
}

ps3d::GameReport Maze3D::Game::start()
{
	ps3d::GameReport report;
	return report;
}

ps3d::GameReport Maze3D::Game::tick(double frameTime)
{
	this->frameTime = frameTime;
	ps3d::GameReport report;
	return report;
}

std::string Maze3D::Game::getName()
{
	return "3D Maze";
}

ps3d::Player* Maze3D::Game::getPlayer()
{
	return &player;
}

ps3d::Map* Maze3D::Game::getMap()
{
	return map;
}

Maze3D::Game::~Game()
{
	delete map;
}

