#include "Walls.h"

ps3d::Wall ps3d::Wall::EMPTY_WALL(nullptr, 0, false, false);

ps3d::Wall::Wall(Texture* texture, int offset, bool visible, bool collideable) : Tickable()
{
	this->texture = texture;
	this->visible = visible;
	this->collideable = collideable;
	this->offset = offset;
}

void ps3d::Wall::setNextOffset(int by)
{
	if (by == 0)
	{
		by = texture->getWidth();
	}
	offset = (offset + by) % texture->getSfTexture()->getSize().x;
}

ps3d::AnimatedWall::AnimatedWall(Texture* texture, float speed, int animationOffsetChange, int offset, bool visible, bool collideable) : Wall(texture, offset, visible, collideable)
{
	this->speed = speed;
	this->animationOffsetChange = animationOffsetChange;
	if (animationOffsetChange == 0)
	{
		this->animationOffsetChange = texture->getWidth();
	}
	this->time = 0;
}

void ps3d::AnimatedWall::tick(double frameTime)
{
	animate(frameTime);
	Wall::tick(frameTime);
}

void ps3d::AnimatedWall::animate(double frameTime)
{
	time += frameTime;
	if (time > 1 / speed)
	{
		setNextOffset(animationOffsetChange);
		time = 0;
	}
}
