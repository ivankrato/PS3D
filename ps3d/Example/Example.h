#pragma once
#include "../Interfaces.h"
#include "../GameReport.h"
#include "../Map.h"
#include "../Texture.h"
#include "../Player.h"
#include "../Settings.h"
#include <map>
#include <random>

namespace Example
{
	class Game : public ps3d::IGame
	{
		std::map<std::string,ps3d::Texture*> textures;
		ps3d::Player player;
		ps3d::Map *map;
		ps3d::MiniMap *miniMap;
		double frameTime;
		bool isEnd;
		ps3d::Settings *settings;
		sf::Vector2i oldPlayerCoords;
		void createMinimap();
		void updateMinimap();

		int score;
		ps3d::GameReport report;
	public:
		Game();
		void start() override;
		ps3d::GameReport tick(double frameTime) override;
		std::string getName() override;
		ps3d::Player *getPlayer() override;
		ps3d::Map* getMap() override;
		ps3d::MiniMap* getMiniMap() override;
		ps3d::Settings* getSettings() override;
		~Game();
	};
}
