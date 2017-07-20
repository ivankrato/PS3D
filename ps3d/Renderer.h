#pragma once
#include <SFML/Graphics.hpp>
#include "Map.h"
#include "Player.h"
#include "GameReport.h"

namespace ps3d
{
	class Renderer
	{
		sf::RenderWindow *window;
		Map *map;
		MiniMap *miniMap;
		Player *player;
		sf::Font *font;
		void renderBG();
		void renderWallsSprites();
		void renderHUD(GameReport gameReport);
		void drawFPS(int fps);
	public:
		Renderer(sf::RenderWindow *window, Map *map, Player *player, MiniMap *miniMap);
		void render(GameReport gameReport, int fps = -1);
	};
}
