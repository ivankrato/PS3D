#pragma once
#include <SFML/Graphics.hpp>
#include "Map.h"
#include "Player.h"
#include "Texture.h"

namespace ps3d
{
	class Renderer
	{
		sf::RenderWindow *window;
		Map *map;
		Player *player;
		void renderBG();
		void renderWallsSprites();
		void renderHUD();
	public:
		Renderer(sf::RenderWindow *window, Map *map, Player *player);
		void render();
	};
}
