#pragma once
#include <functional>

namespace ps3d
{
	class Tickable
	{
		std::function<void(double)> tickFunc;
	public:
		/**
		 * \brief Constructor
		 */
		Tickable();
		/**
		 * \brief sets function which is called each tick
		 * \param tickFunc function to be called each tick, takes one double as a parameter - time of a frame
		 */
		void setOnTick(std::function<void(double)> tickFunc);
		/**
		 * \brief function to be called each tick
		 * \param frameTime time of a frame
		 */
		virtual void tick(double frameTime);
		/**
		 * \brief Destructor
		 */
		virtual ~Tickable() = default;
	};

	class Player;
	class Map;
	class MiniMap;
	class Settings;
	struct GameReport;

	class IGame
	{
	public:
		/**
		 * \brief Constructor
		 */
		IGame() = default;
		/**
		 * \brief Destructor
		 */
		virtual ~IGame() = default;
		/**
		 * \brief function to be called at the start of the game
		 */
		virtual void start() = 0;
		/**
		 * \brief function to be called each tick
		 * \param frameTime time of a frame
		 * \return GameReport instance
		 */
		virtual GameReport tick(double frameTime) = 0;
		/**
		 * \return name of the game
		 */
		virtual std::string getName() = 0;
		/**
		 * \return Player instance
		 */
		virtual Player *getPlayer() = 0;
		/**
		 * \return current map
		 */
		virtual Map *getMap() = 0;
		/**
		 * \return minimap to be shown
		 */
		virtual MiniMap* getMiniMap() = 0;
		/**
		 * \return parsed settings from file
		 */
		virtual Settings* getSettings() = 0;
	};

}
