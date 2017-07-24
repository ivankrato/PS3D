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
	{
		sf::RenderWindow *window;
		Renderer *renderer;
		sf::Clock *clock;

		IGame *game;
		double frameTime;

		float moveSpeed;
		float rotateSpeed;

		Player *player;
		Map *map;
		MiniMap *miniMap;

		GameReport curGameReport;
		void tick();
	public:
		/**
		 * \brief Constructor
		 * \param game implementation of ps3d::IGame to start
		 */
		explicit Engine(ps3d::IGame *game);


		/**
		 * \brief Checks collision between sprite and coordinates
		 * \param sprite sprite to check collision againts
		 * \param coords coordinates of the second item to check collision againts
		 * \return true if collision happens
		 */
		static bool isCollision(Sprite *sprite, sf::Vector2f coords);
		/**
		 * \brief Checks collision between two sets of coordinates
		 * \param coords1 coordinates of the first item to check collision againts
		 * \param coords2 coordinates of the second item to check collision againts
		 * \return true if collision happens
		 */
		static bool isCollision(sf::Vector2f coords1, sf::Vector2f coords2);

		
		/**
		 * \brief sets player's move speed
		 * \param moveSpeed move speed to set, in square/second
		 */
		void setMoveSpeed(float moveSpeed);
		/**
		 * \brief sets player's rotation speed
		 * \param rotateSpeed rotation speed to set, in radians/second
		 */
		void setRotateSpeed(float rotateSpeed);
		/**
		 * \brief prepares the engine and starts the game
		 */
		virtual void start();
	};
}
