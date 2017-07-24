#pragma once
#include <string>
#include <SFML/Graphics.hpp>

namespace ps3d
{
	struct GameReport
	{
		/**
		 * \brief Constructor
		 */
		GameReport();
		/**
		 * \brief true if ending screen should apper
		 */
		bool isEnd;
		/**
		 * \brief size of the text in the top left corner
		 */
		int topLeftSize;
		/**
		 * \brief text in the top left cornder
		 */
		std::string topLeftText;
		/**
		 * \brief size of the text in the bottom left corner
		 */
		int bottomLeftSize;
		/**
		* \brief text in the bottom left cornder
		*/
		std::string bottomLeftText;
		/**
		* \brief size of the text in the bottom right corner
		*/
		int bottomRightSize;
		/**
		* \brief text in the top right corner
		*/
		std::string bottomRightText;
		/**
		 * \brief Color of the ending screen background
		 */
		sf::Color endColor;
		/**
		 * \brief size of the ending screen big text
		 */
		int endBigSize;
		/**
		 * \brief ending screen big text
		 */
		std::string endBigText;
		/**
		* \brief size of the ending screen small text
		*/
		int endSmallSize;
		/**
		* \brief ending screen small text
		*/
		std::string endSmallText;
	};
}
