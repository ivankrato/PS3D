#pragma once
#include <string>
#include <SFML/Graphics.hpp>

namespace ps3d
{
	struct GameReport
	{
		GameReport();
		bool isEnd;
		int topLeftSize;
		std::string topLeftText;
		int bottomLeftSize;
		std::string bottomLeftText;
		int bottomRightSize;
		std::string bottomRightText;
		sf::Color endColor;
		int endBigSize;
		std::string endBigText;
		int endSmallSize;
		std::string endSmallText;
	};
}
