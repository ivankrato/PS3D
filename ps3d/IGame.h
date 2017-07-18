#pragma once
#include "GameReport.h"
#include <string>
#include "Player.h"
#include "Map.h"

namespace ps3d
{
	class IGame
	{
	public:
		IGame() = default;
		virtual ~IGame() = default;
		virtual GameReport start() = 0;
		virtual GameReport tick() = 0;
		virtual std::string getName() = 0;
		virtual Player *getPlayer() = 0;
		virtual Map *getMap() = 0;
	};
}
