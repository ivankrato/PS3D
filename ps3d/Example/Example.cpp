#include "../Walls.h"
#include "../Sprites.h"
#include "Example.h"
#include "../Engine.h"

static const int DEFAULT_MAP_WIDTH = 10;
static const int DEFAULT_MAP_HEIGHT = 10;

Example::Game::Game() : IGame()
{
	isEnd = false;
	frameTime = 0;
	textures.clear();
	// load textures
	textures["walls"] = new ps3d::Texture("Example/Resources/walls.png", false, 512);
	textures["skybox"] = new ps3d::Texture("Example/Resources/skybox.jpg", true);
	textures["skyboxPick"] = new ps3d::Texture("Example/Resources/skyboxPick.png", true);
	textures["chest"] = new ps3d::Texture("Example/Resources/chest.png", false, 128);
	textures["bat"] = new ps3d::Texture("Example/Resources/bat.png", false, 32);
	settings = new ps3d::Settings("Example/conf.ini");

	map = new ps3d::Map(sf::Vector2i(DEFAULT_MAP_WIDTH, DEFAULT_MAP_HEIGHT), &player);
	miniMap = new ps3d::MiniMap(map->getSize());
	score = 0;
}

void Example::Game::start()
{
	// create map
	player.x = 1.5f;
	player.y = 1.5f;
	oldPlayerCoords.x = int(player.x);
	oldPlayerCoords.y = int(player.y);
	for(int i = 0; i < map->getSize().x; i++)
	{
		if(i == 0 || i == map->getSize().x - 1)
		{
			for (int j = 0; j < map->getSize().y; j++)
			{
				ps3d::Wall *wall = new ps3d::Wall(textures["walls"], 1536);
				map->addWall(wall, sf::Vector2i(i, j));
			}
		} else
		{
			ps3d::Wall *wall = new ps3d::Wall(textures["walls"], 1024);
			map->addWall(wall, sf::Vector2i(i, 0));
			wall = new ps3d::Wall(textures["walls"], 1024);
			map->addWall(wall, sf::Vector2i(i, map->getSize().y - 1));
		}
	}
	// sprites
	ps3d::Sprite *scoreChest = new ps3d::Sprite(5.5, 5.5, textures["chest"], false);
	scoreChest->setOnCollision([scoreChest, this]()
	{
		score += 100;
		scoreChest->setNextOffset();
		scoreChest->setOnCollision(nullptr);
	});
	map->addSprite(scoreChest);

	ps3d::Sprite *skyboxPick = new ps3d::Sprite(5.5, 6.5, textures["skyboxPick"], false);
	skyboxPick->setOnCollision([skyboxPick, this]()
	{
		map->removeSprite(skyboxPick);
		map->setSkybox(textures["skybox"]);
	});
	map->addSprite(skyboxPick);

	ps3d::Wall *animatedWall = new ps3d::AnimatedWall(textures["walls"], 100, 5);
	map->addWall(animatedWall, sf::Vector2i(5, 4));

	ps3d::AnimatedMovingSprite *animatedMovingSprite = new ps3d::AnimatedMovingSprite(8.5, 8.5, textures["bat"], 3, true, 10, false);
	animatedMovingSprite->addMove(sf::Vector2f(8.5, 2.5));
	animatedMovingSprite->addMove(sf::Vector2f(2.5, 2.5));
	animatedMovingSprite->addMove(sf::Vector2f(2.5, 8.5));
	animatedMovingSprite->addMove(sf::Vector2f(8.5, 8.5));
	animatedMovingSprite->setOnCollision([this]()
	{
		report.endBigSize = 64;
		report.endBigText = "GAME OVER";
		report.endSmallText = "You died";
		isEnd = true;
	});
	map->addSprite(animatedMovingSprite);

	createMinimap();
}

ps3d::GameReport Example::Game::tick(double frameTime)
{
	this->frameTime = frameTime;

	updateMinimap();
	report.bottomLeftText = std::to_string(1.0f / frameTime);
	report.topLeftSize = 48;
	report.topLeftText = "Example";
	report.bottomRightText = "Score:\n" + std::to_string(score);
	report.isEnd = isEnd;
	return report;
}

std::string Example::Game::getName()
{
	return "3D Maze";
}

ps3d::Player* Example::Game::getPlayer()
{
	return &player;
}

ps3d::Map* Example::Game::getMap()
{
	return map;
}

ps3d::MiniMap* Example::Game::getMiniMap()
{
	return miniMap;
}

ps3d::Settings* Example::Game::getSettings()
{
	return settings;
}

Example::Game::~Game()
{
	delete map;
}

void Example::Game::createMinimap()
{
	miniMap->addColorNumber(1, sf::Color::Black);
	miniMap->addColorNumber(2, sf::Color::White);
	ps3d::Wall ***mapArray = map->getWallArray();
	sf::Vector2i mapSize = map->getSize();
	for (int i = 0; i < mapSize.x; i++)
	{
		for (int j = 0; j < mapSize.y; j++)
		{
			ps3d::Wall *wall = map->getWall(sf::Vector2i(i, j));
			if (wall != nullptr && wall->visible)
			{
				miniMap->setColor(1, sf::Vector2i(i, j));
			}
		}
	}
}

void Example::Game::updateMinimap()
{
	if (oldPlayerCoords.x >= 0
		&& oldPlayerCoords.y >= 0) {
		miniMap->setColor(0, sf::Vector2i(oldPlayerCoords.x, oldPlayerCoords.y));
		miniMap->setColor(2, sf::Vector2i(int(player.x), int(player.y)));
	}
	oldPlayerCoords.x = int(player.x);
	oldPlayerCoords.y = int(player.y);
}
