#include "Maze3D.h"

Maze3D::Game::Game() : IGame()
{
	textures.empty();
	// load textures
	textures["wallText1"] = new ps3d::Texture("Maze3D/Resources/wall1.png");
	textures["wallText2"] = new ps3d::Texture("Maze3D/Resources/wall2.png");
	textures["wallText3"] = new ps3d::Texture("Maze3D/Resources/wall3.png");
	textures["wallText4"] = new ps3d::Texture("Maze3D/Resources/wall4.png");
	textures["skybox"] = new ps3d::Texture("Maze3D/Resources/skybox.jpg", true);
	textures["spriteBarrel"] = new ps3d::Texture("Maze3D/Resources/sprite_barrel.png");
	// TODO random?
	map = new ps3d::Map(sf::Vector2i(30, 30), textures["skybox"]);
	int mapWidth = map->getSize().x;
	int mapHeight = map->getSize().y;
	for (int i = 0; i < mapWidth; i++)
	{
		if (i == 0 || i == mapWidth - 1)
		{
			for (int j = 0; j < mapHeight; j++)
			{
				map->addWall(new ps3d::Wall(textures["wallText1"]), sf::Vector2i(i, j));
			}
		}
		else
		{
			map->addWall(new ps3d::Wall(textures["wallText1"]), sf::Vector2i(i, 0));
			map->addWall(new ps3d::Wall(textures["wallText1"]), sf::Vector2i(i, mapHeight - 1));
		}
	}
}

ps3d::GameReport Maze3D::Game::start()
{
	ps3d::GameReport report;
	return report;
}

ps3d::GameReport Maze3D::Game::tick()
{
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

