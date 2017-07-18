#pragma once
#include "Walls.h"
#include "Sprites.h"
#include <vector>
#include <SFML/Graphics.hpp>

namespace ps3d
{
	class Map
	{
		sf::Vector2i size;
		Wall ***wallArray;
		std::vector<Sprite*> *sprites;
		Texture *skybox;
		sf::Color floorColor;
		sf::Color ceilingColor;
	public:
		Map(sf::Vector2i size, Texture *skybox = nullptr, sf::Color floorColor = sf::Color(0x3D, 0x29, 0x18), sf::Color ceilingColor = sf::Color::Black, std::vector<Sprite*> *sprites = nullptr);
		sf::Vector2i getSize() const;
		Wall ***getWallArray() const;
		Wall *getWall(sf::Vector2i coords) const;
		std::vector<Sprite*> *getSprites() const;
		void addSprite(Sprite *sprite);
		void removeSprite(Sprite *sprite);
		Texture *getSkybox() const;
		void addWall(Wall *wall, sf::Vector2i coords);
		void removeWall(sf::Vector2i coords);
		bool willCollideX(float newPosX, float oldPosY);
		bool willCollideY(float newPosY, float oldPosX);
		sf::Color getFloorColor() const;
		sf::Color getCeilingColor() const;
		bool hasSkybox() const;
		~Map();
	};
}
