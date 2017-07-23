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
