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
		float x;
		float y;
		Player(float x = 3, float y = 3, float rotateBy = 0);
		float getDirX() const;
		float getDirY() const;
		float getPlaneX() const;
		float getPlaneY() const;

		//rotate camera direction and camera plane
		void rotate(float speed);
	};
}
