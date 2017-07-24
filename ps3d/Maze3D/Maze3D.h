#pragma once
#include "../Interfaces.h"
#include "../GameReport.h"
#include "../Map.h"
#include "../Texture.h"
#include "../Player.h"
#include "../Settings.h"
#include <map>
#include <string>
#include <random>

namespace Maze3D
{
	class MapGenerator
	{
		struct Node
		{
			/**
			 * \brief x coordinate
			 */
			int x;
			/**
			 * \brief y coordinate
			 */
			int y;
			/**
			 * \brief true if the node is a wall
			 */
			int isWall;
			/**
			 * \brief constructor
			 * \param x x coordinate
			 * \param y y coordinate
			 */
			Node(int x, int y);
			/**
			 * \param nodeArray array of nodes
			 * \param arrWidth width of the array of nodes
			 * \param arrHeight height of the array of nodes
			 * \return return walls adjacament to this node
			 */
			std::vector<Node*> getAdjWalls(Node*** nodeArray, int arrWidth, int arrHeight);
			/**
			* \param nodeArray array of nodes
			* \param arrWidth width of the array of nodes
			* \param arrHeight height of the array of nodes
			* \return return cells adjacament to this node
			*/
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
		/**
		 * \brief 
		 * \param map level map to generate into
		 * \param seed RNG seed
		 * \param textures wall textures
		 */
		MapGenerator(ps3d::Map *map, int seed, std::map<std::string, ps3d::Texture*> &textures);
		/**
		 * \brief generates the map
		 * \return array of start, end, map pickup coordinates, in this order
		 */
		sf::Vector2i* generate();
		/**
		 * \brief destructor
		 */
		~MapGenerator();
	};

	class Game : public ps3d::IGame
	{
		std::map<std::string,ps3d::Texture*> textures;
		ps3d::Player player;
		ps3d::Map *map;
		ps3d::MiniMap *miniMap;
		double frameTime;
		ps3d::GameReport report;
		bool isEnd;
		ps3d::Settings *settings;
		sf::Vector2i oldPlayerCoords;
		void createMinimap();
		void updateMinimap();
		void showMap();
		int seed;
	public:
		/**
		 * \brief constructor
		 */
		Game();
		void start() override;
		ps3d::GameReport tick(double frameTime) override;
		std::string getName() override;
		ps3d::Player *getPlayer() override;
		ps3d::Map* getMap() override;
		ps3d::MiniMap* getMiniMap() override;
		ps3d::Settings* getSettings() override;
		/**
		 * \brief destructor
		 */
		~Game();
	};
}
