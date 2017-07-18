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
		explicit Texture(std::string file, bool repeated = false, int realWidth = 0, int realHeight = 0);
		int getWidth();
		int getHeight();
		sf::Texture *getSfTexture();
	};
}
