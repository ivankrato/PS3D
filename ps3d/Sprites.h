#pragma once
#include <SFML/System.hpp>
#include "Interfaces.h"
#include "Texture.h"

namespace ps3d
{
	struct Sprite : Tickable
	{
		Texture *texture;
		float x;
		float y;
		float dist;
		bool visible;
		bool collideable;
		int offset;

		explicit Sprite(float x, float y, Texture *texture,	int offset = 0, bool visible = true, bool collideable = true);
		sf::Vector2f* getCollisionSquare();
		void setNextOffset(int by = 0);
		bool hasOnCollision();
		void setOnCollision(std::function<void()> onCollisionEvent);
		void onCollision();

		static bool compareByDist(Sprite *a, Sprite *b);
	private:
		sf::Vector2f collisionSquare[2];
		std::function<void()> onCollisionEvent;
	};

	struct AnimatedSprite : Sprite
	{
		float speed;

		explicit AnimatedSprite(float x, float y, Texture *texture, float speed, int animationOffsetChange = 0, int offset = 0, bool visible = true, bool collideable = true);
		void tick(double frameTime) override;
		void animate(double frameTime);
	private:
		int animationOffsetChange;
		double time;
	};

	struct MovingSprite : Sprite
	{

	};

	struct AnimatedMovingSprite : AnimatedSprite, MovingSprite
	{

	};
}
