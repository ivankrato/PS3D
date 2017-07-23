#include "Maze3D.h"
#include "../Walls.h"
#include "../Sprites.h"
#include <iostream>

static const int DEFAULT_MAP_WIDTH = 50;
static const int DEFAULT_MAP_HEIGHT = 50;

Maze3D::MapGenerator::Node::Node(int x, int y)
{
	this->x = x;
	this->y = y;
	this->isWall = true;
}

std::vector<Maze3D::MapGenerator::Node*> Maze3D::MapGenerator::Node::getAdjWalls(Node*** nodeArray, int arrWidth, int arrHeight)
{
	std::vector<Node*> adjWalls;
	if (x - 1 >= 0 && nodeArray[x - 1][y]->isWall) adjWalls.push_back(nodeArray[x - 1][y]);
	if (x + 1 < arrWidth && nodeArray[x + 1][y]->isWall) adjWalls.push_back(nodeArray[x + 1][y]);
	if (y - 1 >= 0 && nodeArray[x][y - 1]->isWall) adjWalls.push_back(nodeArray[x][y - 1]);
	if (y + 1 < arrHeight && nodeArray[x][y + 1]->isWall) adjWalls.push_back(nodeArray[x][y + 1]);
	return adjWalls;
}

std::vector<Maze3D::MapGenerator::Node*> Maze3D::MapGenerator::Node::getAdjCells(Node*** nodeArray, int arrWidth, int arrHeight)
{
	std::vector<Node*> adjWalls;
	if (x - 1 >= 0 && !nodeArray[x - 1][y]->isWall) adjWalls.push_back(nodeArray[x - 1][y]);
	if (x + 1 < arrWidth && !nodeArray[x + 1][y]->isWall) adjWalls.push_back(nodeArray[x + 1][y]);
	if (y - 1 >= 0 && !nodeArray[x][y - 1]->isWall) adjWalls.push_back(nodeArray[x][y - 1]);
	if (y + 1 < arrHeight && !nodeArray[x][y + 1]->isWall) adjWalls.push_back(nodeArray[x][y + 1]);
	return adjWalls;
}

Maze3D::MapGenerator::MapGenerator(ps3d::Map *map, int seed, std::map<std::string, ps3d::Texture*> &textures) : rng(seed)
{
	this->map = map;
	this->textures = textures;
	this->width = map->getSize().x - 2;
	this->height = map->getSize().y - 2;
	nodeArray = new Node**[width];
	for (int i = 0; i < width; i++)
	{
		nodeArray[i] = new Node*[height];
		for (int j = 0; j < height; j++)
		{
			nodeArray[i][j] = new Node(i, j);
		}
	}
}

sf::Vector2i* Maze3D::MapGenerator::generate()
{
	sf::Vector2i *generatedCoords = new sf::Vector2i[3];
	std::uniform_int_distribution<int> widthUni(0, width - 1);
	std::uniform_int_distribution<int> heightUni(0, height - 1);
	int startX = widthUni(rng);
	int startY = 0;
	generatedCoords[0].x = 1 + startX; // 1 because we need border
	generatedCoords[0].y = 1 + startY; // 1 because we need border
	// PRIM ALGORITHM
	// pick starting wall
	Node *randomNode = nodeArray[startX][startY];
	// make part of the maze
	randomNode->isWall = false;
	// add the adjacent walls to list
	std::vector<Node*> adjWalls = randomNode->getAdjWalls(nodeArray, width, height);
	adjWallList.insert(adjWallList.end(), adjWalls.begin(), adjWalls.end());
	while (!adjWallList.empty())
	{
		// pick random wall
		std::uniform_int_distribution<int> uni(0, int(adjWallList.size()) - 1);
		int randomNodeIndex = uni(rng);
		randomNode = adjWallList[randomNodeIndex];
		std::vector<Node*> adjCells = randomNode->getAdjCells(nodeArray, width, height);
		// if there is only one adjacent cell
		if (adjCells.size() == 1)
		{
			// make part of the maze
			randomNode->isWall = false;
			// add adjacent walls to the wall list
			adjWalls = randomNode->getAdjWalls(nodeArray, width, height);
			adjWallList.insert(adjWallList.end(), adjWalls.begin(), adjWalls.end());
		}
		// remove from the list
		adjWallList.erase(adjWallList.begin() + randomNodeIndex);
	}
	// pick ending
	bool endingPicked = false;
	while (!endingPicked)
	{
		int endX = widthUni(rng);
		int endY = height;
		generatedCoords[1].x = 1 + endX;
		generatedCoords[1].y = 1 + endY;
		endingPicked = !nodeArray[endX][endY - 1]->isWall;
	}
	// pick minimap pickup
	bool mapPicked = false;
	while (!mapPicked)
	{
		int mapX = widthUni(rng);
		int mapY = heightUni(rng);
		generatedCoords[2].x = 1 + mapX;
		generatedCoords[2].y = 1 + mapY;
		mapPicked = !nodeArray[mapX][mapY]->isWall;
	}
	// add walls to the map
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			if (nodeArray[i][j]->isWall)
			{
				map->addWall(new ps3d::Wall(textures["walls"]), sf::Vector2i(i + 1, j + 1));
			}
		}
	}
	// add border
	for (int i = 0; i < width + 2; i++)
	{
		if (i == 0 || i == width + 1)
		{
			for (int j = 0; j < height + 2; j++)
			{
				if (i != generatedCoords[1].x || j != generatedCoords[1].y)
				{
					map->addWall(new ps3d::Wall(textures["walls"]), sf::Vector2i(i, j));
				}
			}
		}
		else
		{
			map->addWall(new ps3d::Wall(textures["walls"]), sf::Vector2i(i, height + 1));
			map->addWall(new ps3d::Wall(textures["walls"]), sf::Vector2i(i, 0));
		}
	}
	// change textures
	std::uniform_int_distribution<int> fullWidthUni(0, width + 2);
	std::uniform_int_distribution<int> fullHeightUni(0, height + 2);
	for (int n = 1; n < 4; n++)
	{
		int w1 = fullWidthUni(rng);
		int w2 = fullWidthUni(rng);
		int h1 = fullHeightUni(rng);
		int h2 = fullHeightUni(rng);
		for (int i = std::min(w1, w2); i < std::max(w1, w2); i++)
		{
			for (int j = std::min(h1, h2); j < std::max(h1, h2); j++)
			{
				ps3d::Wall *wall = map->getWall(sf::Vector2i(i, j));
				if (wall != nullptr)
				{
					wall->setNextOffset(n*wall->texture->getWidth());
				}
			}
		}
	}
	map->removeWall(sf::Vector2i(generatedCoords[1].x, generatedCoords[1].y));
	return generatedCoords;
}

Maze3D::MapGenerator::~MapGenerator()
{
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++)
		{
			delete nodeArray[i][j];
		}
		delete[] nodeArray[i];
	}
	delete[] nodeArray;
}

Maze3D::Game::Game() : IGame(), oldPlayerCoords(-1, -1)
{
	isEnd = false;
	frameTime = 0;
	textures.clear();
	// load textures
	textures["walls"] = new ps3d::Texture("Maze3D/Resources/walls.png", false, 512);
	textures["skybox"] = new ps3d::Texture("Maze3D/Resources/skybox.jpg", true);
	textures["map"] = new ps3d::Texture("Maze3D/Resources/map.png", false);
	textures["endFlag"] = new ps3d::Texture("Maze3D/Resources/end_flag.png", false, 512);

	map = new ps3d::Map(sf::Vector2i(DEFAULT_MAP_WIDTH, DEFAULT_MAP_HEIGHT), &player, textures["skybox"]);
	miniMap = new ps3d::MiniMap(map->getSize());
}

ps3d::GameReport Maze3D::Game::start()
{
	// generate map
	std::random_device rd;
	MapGenerator mapGenerator(map, rd(), textures);
	sf::Vector2i *generatedCoords = mapGenerator.generate();
	player.x = generatedCoords[0].x + 0.5f;
	player.y = generatedCoords[0].y + 0.5f;
	// end
	sf::Vector2f end(generatedCoords[1].x + 0.5f, generatedCoords[1].y + 0.5f);
	ps3d::Sprite *endSprite = new ps3d::AnimatedSprite(end.x, end.y, textures["endFlag"], 8, 0, 0, true, false);
	endSprite->setOnCollision([this]()
	{
		this->isEnd = true;
	});
	map->addSprite(endSprite);
	// map pickup
	ps3d::Sprite *mapSprite = new ps3d::Sprite(generatedCoords[2].x + 0.5f, generatedCoords[2].y + 0.5f, textures["map"], 0, true, false);
	mapSprite->setOnCollision([this, mapSprite]()
	{
		this->showMap();
		this->map->removeSprite(mapSprite);
		delete mapSprite;
	});
	map->addSprite(mapSprite);
	delete[] generatedCoords;

	createMinimap();

	ps3d::GameReport report;
	return report;
}

ps3d::GameReport Maze3D::Game::tick(double frameTime)
{
	this->frameTime = frameTime;

	updateMinimap();

	ps3d::GameReport report;
	report.isEnd = isEnd;
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

ps3d::MiniMap* Maze3D::Game::getMiniMap()
{
	return miniMap;
}

std::string Maze3D::Game::getConfFileName()
{
	return "Maze3D/conf.ini";
}

Maze3D::Game::~Game()
{
	delete map;
}

void Maze3D::Game::createMinimap()
{
	miniMap->addColorNumber(1, sf::Color::Black);
	miniMap->addColorNumber(2, sf::Color::White);
	miniMap->addColorNumber(3, sf::Color(255, 255, 255, 100));
}

void Maze3D::Game::updateMinimap()
{
	if (oldPlayerCoords.x >= 0
		&& oldPlayerCoords.y >= 0) {
		miniMap->setColor(3, sf::Vector2i(oldPlayerCoords.x, oldPlayerCoords.y));
		miniMap->setColor(2, sf::Vector2i(int(player.x), int(player.y)));
	}
	oldPlayerCoords.x = int(player.x);
	oldPlayerCoords.y = int(player.y);
}

void Maze3D::Game::showMap()
{
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
