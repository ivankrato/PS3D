#include "Sprites.h"

extern const float COLLISION_RADIUS;

ps3d::Sprite::Sprite(float x, float y, Texture* texture, int offset, bool visible, bool collideable) : Tickable()
{
	this->x = x;
	this->y = y;
	collisionSquare[0] = sf::Vector2f(x - COLLISION_RADIUS, y - COLLISION_RADIUS);
	collisionSquare[1] = sf::Vector2f(x + COLLISION_RADIUS, y + COLLISION_RADIUS);
	this->texture = texture;
	this->offset = offset;
	this->visible = visible;
	this->collideable = collideable;
	this->dist = 0;
	onCollisionEvent = nullptr;
}

sf::Vector2f* ps3d::Sprite::getCollisionSquare()
{
	return collisionSquare;
}

void ps3d::Sprite::setNextOffset(int by)
{
	if (by == 0)
	{
		by = texture->getWidth();
	}
	offset = (offset + by) % texture->getSfTexture()->getSize().x;
}

bool ps3d::Sprite::hasOnCollision()
{
	return onCollisionEvent != nullptr;
}

void ps3d::Sprite::setOnCollision(std::function<void()> onCollisionEvent)
{
	this->onCollisionEvent = onCollisionEvent;
}

void ps3d::Sprite::onCollision()
{
	if(onCollisionEvent != nullptr)
	{
		onCollisionEvent();
	}
}

bool ps3d::Sprite::compareByDist(Sprite* a, Sprite* b)
{
	return a->dist > b->dist;
}

ps3d::AnimatedSprite::AnimatedSprite(float x, float y, Texture* texture, float speed, int offsetChange, int offset, bool visible, bool collideable) : Sprite(x, y, texture, offset, visible, collideable)
{
	this->speed = speed;
	this->animationOffsetChange = offsetChange;
	if(offsetChange == 0)
	{
		this->animationOffsetChange = texture->getWidth();
	}
	this->time = 0;
}

void ps3d::AnimatedSprite::tick(double frameTime)
{
	animate(frameTime);
	Sprite::tick(frameTime);
}

void ps3d::AnimatedSprite::animate(double frameTime)
{
	time += frameTime;
	if(time > 1 / speed)
	{
		setNextOffset(animationOffsetChange);
		time = 0;
	}
}
