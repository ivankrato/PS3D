#include "Walls.h"

ps3d::Wall::Wall(Texture* texture, bool visible, bool collideable)
{
	this->texture = texture;
	this->visible = visible;
	this->collideable = collideable;
}

ps3d::EventWall::EventWall(Texture* texture, bool visible, bool collideable) : Wall(texture, visible, collideable)
{
}
