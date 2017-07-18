#pragma once
#include "../IGame.h"
#include "../GameReport.h"
#include "../Map.h"
#include "../Texture.h"
#include "../Walls.h"
#include "../Sprites.h"
#include "../Player.h"

namespace Maze3D
{
	class Game : public ps3d::IGame
	{
		std::map<std::string,ps3d::Texture*> textures;
		ps3d::Player player;
		ps3d::Map *map;
	public:
		Game();
		ps3d::GameReport start() override;
		ps3d::GameReport tick() override;
		std::string getName() override;
		ps3d::Player *getPlayer() override;
		ps3d::Map* getMap() override;
		~Game();
	};
}
