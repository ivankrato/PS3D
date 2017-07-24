#pragma once
#include <SFML/Graphics.hpp>
#include "Interfaces.h"
#include "Walls.h"
#include "Sprites.h"
#include "Texture.h"
#include "Player.h"
#include <vector>

namespace ps3d
{
	class Map : public Tickable
	{
		Player *player;
		sf::Vector2i size;
		Wall ***wallArray;
		std::vector<Sprite*> *sprites;
		std::vector<Sprite*> spritesFlaggedForRemove;
		void removeFlaggedSprites();
		Texture *skybox;
		sf::Color floorColor;
		sf::Color ceilingColor;
	public:
		/**
		 * \brief Constructor
		 * \param size size of the map
		 * \param player Player instance
		 * \param skybox skybox of the level, nullptr if ceiling shou be used
		 * \param floorColor color of the level floor
		 * \param ceilingColor color of the level ceiling (only if skybox is nullptr)
		 * \param sprites vector of sprites on the map
		 */
		Map(sf::Vector2i size, Player *player, Texture *skybox = nullptr, sf::Color floorColor = sf::Color(0x3D, 0x29, 0x18), sf::Color ceilingColor = sf::Color(100, 100, 100), std::vector<Sprite*> *sprites = nullptr);
		/**
		 * \return size of the map
		 */
		sf::Vector2i getSize() const;
		/**
		 * \return array of map walls
		 */
		Wall ***getWallArray() const;
		/**
		 * \param coords coordinates of the wall to get
		 * \return Wall instance, nullptr if no wall is on the coordinates
		 */
		Wall *getWall(sf::Vector2i coords) const;
		/**
		 * \return vector of sprites on the map
		 */
		std::vector<Sprite*> *getSprites() const;
		/**
		 * \return color of the floor
		 */
		sf::Color getFloorColor() const;
		/**
		 * \return color of the ceiling
		 */
		sf::Color getCeilingColor() const;

		/**
		 * \brief sets the skybox of the level
		 * \param skybox Texture instance of the skybox
		 */
		void setSkybox(Texture* skybox);
		/**
		 * \brief sets the color of the floor
		 * \param floor_color color of the floor to set
		 */
		void setFloorColor(sf::Color floor_color);
		/**
		 * \brief sets the color of the ceiling
		 * \param ceiling_color color of the ceiling to set
		 */
		void setCeilingColor(sf::Color ceiling_color);

		/**
		 * \return Texture instance of the skybox
		 */
		Texture *getSkybox() const;
		/**
		 * \return true if the map has skybox
		 */
		bool hasSkybox() const;

		/**
		 * \brief Adds sprite to the map
		 * \param sprite sprite to be added
		 */
		void addSprite(Sprite *sprite);
		/**
		 * \brief Removes sprite from map
		 * \param sprite sprite to be removed
		 */
		void removeSprite(Sprite *sprite);
		/**
		 * \brief Adds wall to the map
		 * \param wall wall to be added
		 * \param coords coords of the wall
		 */
		void addWall(Wall *wall, sf::Vector2i coords);
		/**
		 * \brief removes wall from the map
		 * \param coords coords of the wall to be removed
		 */
		void removeWall(sf::Vector2i coords);

		/**
		 * \brief checks if something can move somewhere
		 * \param newPos new coordinates
		 * \param oldPos old coordinates
		 * \return Vector2 of bools, x and y axis
		 */
		sf::Vector2<bool> canMove(sf::Vector2f newPos, sf::Vector2f oldPos);
		/**
		 * \brief function to be called each tick
		 * \param frameTime time of a frame
		 */
		void tick(double frameTime) override;
		/**
		 * \brief destructor
		 */
		~Map();
	};

	class MiniMap
	{
		std::map<int, sf::Color> colors;
		int **minimapArray;
		int width;
		int height;
	public:
		MiniMap(sf::Vector2i size);
		sf::Color getColor(sf::Vector2i coords);
		void addColorNumber(int i, sf::Color color);
		void setColor(int color, sf::Vector2i coords);
		sf::Vector2i getSize();
	};
}
