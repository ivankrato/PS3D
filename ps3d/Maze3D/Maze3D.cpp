#include "Maze3D.h"
#include "../Walls.h"
#include "../Sprites.h"
#include <iostream>

static const int DEFAULT_MAP_WIDTH = 15;
static const int DEFAULT_MAP_HEIGHT = 15;

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
	sf::Vector2i *startEndCoords = new sf::Vector2i[2];
	std::uniform_int_distribution<int> widthUni(0, width - 1);
	int startX = widthUni(rng);
	int startY = 0;
	startEndCoords[0].x = 1 + startX; // 1 because we need border
	startEndCoords[0].y = 1 + startY; // 1 because we need border
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
		startEndCoords[1].x = 1 + endX;
		startEndCoords[1].y = 1 + endY;
		endingPicked = !nodeArray[endX][endY - 1]->isWall;
	}
	// add walls to the map
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			if (nodeArray[i][j]->isWall)
			{
				map->addWall(new ps3d::Wall(textures["walls"], 0), sf::Vector2i(i + 1, j + 1));
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
				if(i != startEndCoords[1].x || j != startEndCoords[1].y)
				{
					map->addWall(new ps3d::Wall(textures["walls"], 512), sf::Vector2i(i, j));
				}
			}
		}
		else
		{
			map->addWall(new ps3d::Wall(textures["walls"], 512), sf::Vector2i(i, height + 1));
			map->addWall(new ps3d::Wall(textures["walls"], 512), sf::Vector2i(i, 0));
		}
	}
	map->removeWall(sf::Vector2i(startEndCoords[1].x, startEndCoords[1].y));
	return startEndCoords;
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

Maze3D::Game::Game() : IGame()
{
	isEnd = false;
	frameTime = 0;
	textures.clear();
	// load textures
	textures["walls"] = new ps3d::Texture("Maze3D/Resources/walls.png", false, 512);
	textures["skybox"] = new ps3d::Texture("Maze3D/Resources/skybox.jpg", true);
	textures["spriteBarrel"] = new ps3d::Texture("Maze3D/Resources/sprite_barrel.png", false, 512);
	textures["endFlag"] = new ps3d::Texture("Maze3D/Resources/end_flag.png", false);

	map = new ps3d::Map(sf::Vector2i(DEFAULT_MAP_WIDTH, DEFAULT_MAP_HEIGHT), &player, textures["skybox"]);
}

ps3d::GameReport Maze3D::Game::start()
{
	// generate map
	std::random_device rd;
	MapGenerator mapGenerator(map, rd(), textures);
	sf::Vector2i *startEndCoords = mapGenerator.generate();
	player.x = startEndCoords[0].x + 0.5f;
	player.y = startEndCoords[0].y + 0.5f;
	// end
	sf::Vector2f end(startEndCoords[1].x + 0.5f, startEndCoords[1].y + 0.5f);
	ps3d::Sprite *endSprite = new ps3d::Sprite(end.x, end.y, textures["endFlag"], 0, true, false);
	endSprite->setOnCollision([this]()
	{
		this->isEnd = true;
	});
	map->addSprite(endSprite);
	delete[] startEndCoords;

	ps3d::GameReport report;
	return report;
}

ps3d::GameReport Maze3D::Game::tick(double frameTime)
{
	this->frameTime = frameTime;
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

Maze3D::Game::~Game()
{
	delete map;
}

