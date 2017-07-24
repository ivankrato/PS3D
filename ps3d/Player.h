#pragma once
#include "Interfaces.h"
#include <SFML/System/Vector2.hpp>
#include <vector>

namespace ps3d
{
	class Player : public Tickable
	{
		float dirX;
		float dirY;
		float planeX;
		float planeY;
	public:
		/**
		 * \brief x coordinate
		 */
		float x;
		/**
		 * \brief y coordinate
		 */
		float y;
		/**
		 * \brief constructor
		 * \param x starting x coordinate
		 * \param y starting y coordinate
		 * \param rotateBy starting rotation change
		 */
		Player(float x = 0, float y = 0, float rotateBy = 0);
		/**
		 * \return return x coordinate of direction vector 
		 */
		float getDirX() const;
		/**
		 * \return return y coordinate of direction vector
		 */
		float getDirY() const;
		/**
		* \return return x coordinate of camera plane vector
		*/
		float getPlaneX() const;
		/**
		* \return return y coordinate of camera plane vector
		*/
		float getPlaneY() const;
		
		/**
		 * \brief rotate camera direction and camera plane
		 * \param rad radians to rotate
		 */
		void rotate(float rad);
	};
}
