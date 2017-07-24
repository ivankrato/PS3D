#pragma once
#include <SFML/Graphics.hpp>

namespace ps3d
{
	class Texture
	{
		sf::Texture sfTexture;
		int realWidth;
		int realHeight;
	public:
		/**
		 * \brief constructor
		 * \param file file with the texture
		 * \param repeated true if the texture should be repeated
		 * \param realWidth game width of one part of the texture, if 0 it's set to the image width
		 * \param realHeight game height of one part of the texture, if 0 it's set to the image height
		 */
		explicit Texture(std::string file, bool repeated = false, int realWidth = 0, int realHeight = 0);
		/**
		 * \return game width of one part of the texture
		 */
		int getWidth();
		/**
		 * \return  game height of one part of the texture
		 */
		int getHeight();
		/**
		 * \return instance of sf::Texture
		 */
		sf::Texture *getSfTexture();
	};
}
