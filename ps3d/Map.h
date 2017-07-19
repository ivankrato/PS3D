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
		Map(sf::Vector2i size, Player *player, Texture *skybox = nullptr, sf::Color floorColor = sf::Color(0x3D, 0x29, 0x18), sf::Color ceilingColor = sf::Color::Black, std::vector<Sprite*> *sprites = nullptr);
		sf::Vector2i getSize() const;
		Wall ***getWallArray() const;
		Wall *getWall(sf::Vector2i coords) const;
		std::vector<Sprite*> *getSprites() const;
		sf::Color getFloorColor() const;
		sf::Color getCeilingColor() const;
		Texture *getSkybox() const;
		bool hasSkybox() const;

		void addSprite(Sprite *sprite);
		void removeSprite(Sprite *sprite);
		void addWall(Wall *wall, sf::Vector2i coords);
		void removeWall(sf::Vector2i coords);

		sf::Vector2<bool> canMove(sf::Vector2f newPos, sf::Vector2f oldPos);
		void tick(double frameTime) override;
		~Map();
	};
}
