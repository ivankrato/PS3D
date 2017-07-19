﻿#pragma once
#include <functional>

namespace ps3d
{
	class Tickable
	{
		std::function<void(double)> tickFunc;
	public:
		Tickable();
		void setOnTick(std::function<void(double)> tickFunc);
		virtual void tick(double frameTime);
		virtual ~Tickable() = default;
	};

	class Player;
	class Map;
	struct GameReport;

	class IGame
	{
	public:
		IGame() = default;
		virtual ~IGame() = default;
		virtual GameReport start() = 0;
		virtual GameReport tick(double frameTime) = 0;
		virtual std::string getName() = 0;
		virtual Player *getPlayer() = 0;
		virtual Map *getMap() = 0;
	};

}
