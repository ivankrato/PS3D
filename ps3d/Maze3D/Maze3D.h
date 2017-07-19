#pragma once
#include "../Interfaces.h"
#include "../GameReport.h"
#include "../Map.h"
#include "../Texture.h"
#include "../Player.h"
#include <map>
#include <string>
#include <random>

namespace Maze3D
{
	class MapGenerator
	{
		struct Node
		{
			int x;
			int y;
			int isWall;
			Node(int x, int y);
			std::vector<Node*> getAdjWalls(Node*** nodeArray, int arrWidth, int arrHeight);
			std::vector<Node*> getAdjCells(Node*** nodeArray, int arrWidth, int arrHeight);
		};

		int width;
		int height;
		ps3d::Map *map;
		std::map<std::string, ps3d::Texture*> textures;
		std::mt19937 rng;
		Node*** nodeArray;
		std::vector<Node*> adjWallList;
	public:
		MapGenerator(ps3d::Map *map, int seed, std::map<std::string, ps3d::Texture*> &textures);
		sf::Vector2i* generate();
		~MapGenerator();
	};

	class Game : public ps3d::IGame
	{
		std::map<std::string,ps3d::Texture*> textures;
		ps3d::Player player;
		ps3d::Map *map;
		double frameTime;
		bool isEnd;
	public:
		Game();
		ps3d::GameReport start() override;
		ps3d::GameReport tick(double frameTime) override;
		std::string getName() override;
		ps3d::Player *getPlayer() override;
		ps3d::Map* getMap() override;
		~Game();
	};
}
