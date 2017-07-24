#include "Sprites.h"

extern const float COLLISION_RADIUS;

ps3d::Sprite::Sprite(float x, float y, Texture* texture, bool collideable, int offset, bool visible) : Tickable()
{
	this->x = x;
	this->y = y;
	calculateCollisionSquare();
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
	if (onCollisionEvent != nullptr)
	{
		onCollisionEvent();
	}
}

bool ps3d::Sprite::compareByDist(Sprite* a, Sprite* b)
{
	return a->dist > b->dist;
}

void ps3d::Sprite::calculateCollisionSquare()
{
	collisionSquare[0] = sf::Vector2f(x - COLLISION_RADIUS, y - COLLISION_RADIUS);
	collisionSquare[1] = sf::Vector2f(x + COLLISION_RADIUS, y + COLLISION_RADIUS);
}

ps3d::AnimatedSprite::AnimatedSprite(float x, float y, Texture* texture, float speed, bool collideable, int offsetChange, int offset, bool visible) : Sprite(x, y, texture, collideable, offset, visible)
{
	this->animationSpeed = speed;
	this->animationOffsetChange = offsetChange;
	if (offsetChange == 0)
	{
		this->animationOffsetChange = texture->getWidth();
	}
	this->time = 0;
}

void ps3d::AnimatedSprite::tick(double frameTime)
{
	Sprite::tick(frameTime);
	animate(frameTime);
}

void ps3d::AnimatedSprite::animate(double frameTime)
{
	time += frameTime;
	if (time > 1 / animationSpeed)
	{
		setNextOffset(animationOffsetChange);
		time = 0;
	}
}

ps3d::MovingSprite::MovingSprite(float x, float y, Texture* texture, float speed, bool repeat, bool collideable, int offset, bool visible) : Sprite(x, y, texture, collideable, offset, visible)
{
	this->repeat = repeat;
	this->moveSpeed = speed;
	currentMoveTime = 0;
	currentLength = 0;
	first = true;
}

void ps3d::MovingSprite::tick(double frameTime)
{
	Sprite::tick(frameTime);
	move(frameTime);
}

void ps3d::MovingSprite::addMove(sf::Vector2f move)
{
	moveQueue.push(move);
}

sf::Vector2f ps3d::MovingSprite::getNextCoords(double frameTime)
{
	double moveTime = currentLength / moveSpeed;
	float x = this->x + float(diff.x * frameTime / moveTime);
	float y = this->y + float(diff.y * frameTime / moveTime);
	return sf::Vector2f(x, y);
}

void ps3d::MovingSprite::move(double frameTime)
{
	currentMoveTime += frameTime;
	double moveTime = currentLength / moveSpeed;
	if (!moveQueue.empty() && currentMoveTime >= moveTime)
	{
		if (!first)
		{
			x = currentMove.x;
			y = currentMove.y;
		}
		currentMove = moveQueue.front();
		moveQueue.pop();
		if (repeat)
		{
			addMove(currentMove);
		}
		diff = sf::Vector2f(currentMove.x - x, currentMove.y - y);
		currentLength = sqrt(diff.x*diff.x + diff.y*diff.y);
		currentMoveTime = 0;
		first = false;
		moveTime = currentLength / moveSpeed;
	}
	x += float(diff.x * frameTime / moveTime);
	y += float(diff.y * frameTime / moveTime);
	calculateCollisionSquare();
}

ps3d::AnimatedMovingSprite::AnimatedMovingSprite(float x, float y, Texture* texture, float moveSpeed, bool repeat, float animationSpeed, bool collideable, int animationOffsetChange, int offset, bool visible)
	: Sprite(x, y, texture, collideable, offset, visible),
	AnimatedSprite(x, y, texture, animationSpeed, collideable, animationOffsetChange, offset, visible),
	MovingSprite(x, y, texture, moveSpeed, repeat, collideable, offset, visible)
{
}

void ps3d::AnimatedMovingSprite::tick(double frameTime)
{
	Sprite::tick(frameTime);
	animate(frameTime);
	move(frameTime);
}
