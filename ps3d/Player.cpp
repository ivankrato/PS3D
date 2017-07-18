#include "Player.h"
#include <cmath>

static const float DEFAULT_DIR_X = 0;
static const float DEFAULT_DIR_Y = 1;

extern const float DEFAULT_PLANE_WIDTH = 0.66f;
static const float DEFAULT_PLANE_X = DEFAULT_PLANE_WIDTH;
static const float DEFAULT_PLANE_Y = 0;


ps3d::Player::Player(float x, float y, float rotateBy)
{
	this->x = x;
	this->y = y;
	dirX = DEFAULT_DIR_X;
	dirY = DEFAULT_DIR_Y;
	planeX = DEFAULT_PLANE_X;
	planeY = DEFAULT_PLANE_Y;
	rotate(rotateBy);
}

float ps3d::Player::getDirX() const
{
	return dirX;
}

float ps3d::Player::getDirY() const
{
	return dirY;
}

float ps3d::Player::getPlaneX() const
{
	return planeX;
}

float ps3d::Player::getPlaneY() const
{
	return planeY;
}

void ps3d::Player::rotate(float rad)
{
	float oldDirX = dirX;
	dirX = dirX * cos(rad) - dirY * sin(rad);
	dirY = oldDirX * sin(rad) + dirY * cos(rad);
	float oldPlaneX = planeX;
	planeX = planeX * cos(rad) - planeY * sin(rad);
	planeY = oldPlaneX * sin(rad) + planeY * cos(rad);
}
