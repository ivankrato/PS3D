#pragma once
#include "../Interfaces.h"
#include "../GameReport.h"
#include "../Map.h"
#include "../Texture.h"
#include "../Player.h"
#include <map>
#include <string>

namespace Maze3D
{
	class Game : public ps3d::IGame
	{
		std::map<std::string,ps3d::Texture*> textures;
		ps3d::Player player;
		ps3d::Map *map;
		double frameTime;
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
