#pragma once
#include "SFML/Graphics.hpp"
#include "Interfaces.h"
#include "Renderer.h"
#include "Player.h"
#include "Map.h"
#include "GameReport.h"

namespace ps3d
{
	class Engine
		/**
		 * Je třeba nějaká hra
		 * O levely se stará hra
		 * Renderer obsahuje mapu, informace o velikostech textur, skybox, barvy stropu a země, ...
		 * Každá věc na mapš bude obsahovat metodu tick, která normálně nebude nic dělat (jen u animací animovat)
		 * Sprite možná bude abstraktní (hra si ho bude muset extendnou), ale spíš ne - to samé pro Moving sprite - kvůli cestě - bude obsahovat i metodu collision() - při kolizi s hráčem - dá se využít jako PickUp
		 * Třída IGame obsahuje jen virtuální metodu tick() - musí být extendnuta - na tu uvidí Engine - tick bude vracet nějaký GameReport - obsahuje např. informace, které by měl engine zobrazit (skóre, minimapa, životy, ...)
		 * Některý věci - např. Sprite musí vidět na game (kvůli kolizi)
		 * EventWall funguje jako Sprite, ale event() (collision()) se spustí při "stisknutí" zdi
		 * main bude ve hře - vytvoří engine, přiřadí mu Game
		 */
	{
		sf::RenderWindow *window;
		Renderer *renderer;
		sf::Clock *clock;

		IGame *game;
		double frameTime;

		float moveSpeedConst;
		float rotateSpeedConst;

		Player *player;
		Map *map;

		GameReport curGameReport;
		void tick();
	public:
		explicit Engine(ps3d::IGame *game);

		static bool isCollision(Sprite *sprite, sf::Vector2f coords);
		static bool isCollision(sf::Vector2f coords1, sf::Vector2f coords2);

		//the value is in squares/second
		void setMoveSpeed(float moveSpeed);

		//the value is in radians/second
		void setRotateSpeed(float rotateSpeed);
		void start();
	};
}
