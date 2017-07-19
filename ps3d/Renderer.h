#pragma once
#include <SFML/Graphics.hpp>
#include "Map.h"
#include "Player.h"

namespace ps3d
{
	class Renderer
	{
		sf::RenderWindow *window;
		Map *map;
		Player *player;
		sf::Font *font;
		void renderBG();
		void renderWallsSprites();
		void renderHUD();
		void drawFPS(int fps);
	public:
		Renderer(sf::RenderWindow *window, Map *map, Player *player);
		void render(int fps = -1);
	};
}
