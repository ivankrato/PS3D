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
		void renderHUD(GameReport &gameReport);
		void renderEnd(GameReport &gameReport);
		int drawDistance;
		float wallHeightMultiplier;
		sf::Text *textHUD;
		sf::Texture *miniMapTexture;
		sf::Sprite *miniMapSprite;
	public:
		/**
		 * \brief constructor
		 * \param window game window
		 * \param map level map
		 * \param player Player instance
		 * \param miniMap minimap to render
		 * \param settings parsed settings
		 */
		Renderer(sf::RenderWindow *window, Map *map, Player *player, MiniMap *miniMap, Settings *settings);
		/**
		 * \brief renders the graphics
		 * \param gameReport game report to render
		 */
		void render(GameReport &gameReport);
	};
}
