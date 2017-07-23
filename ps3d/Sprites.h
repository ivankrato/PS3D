#pragma once
#include <SFML/System.hpp>
#include "Interfaces.h"
#include "Texture.h"
#include <queue>

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

		explicit Sprite(float x, float y, Texture *texture, bool collideable = true,	int offset = 0, bool visible = true);
		sf::Vector2f* getCollisionSquare();
		void setNextOffset(int by = 0);
		bool hasOnCollision();
		void setOnCollision(std::function<void()> onCollisionEvent);
		void onCollision();

		static bool compareByDist(Sprite *a, Sprite *b);
	protected:
		void calculateCollisionSquare();
	private:
		sf::Vector2f collisionSquare[2];
		std::function<void()> onCollisionEvent;
	};

	struct AnimatedSprite : virtual Sprite
	{
		float animationSpeed;

		AnimatedSprite(float x, float y, Texture *texture, float speed, bool collideable = true, int animationOffsetChange = 0, int offset = 0, bool visible = true);
		void tick(double frameTime) override;
	protected:
		virtual void animate(double frameTime);
	private:
		int animationOffsetChange;
		double time;
	};

	struct MovingSprite : virtual Sprite
	{
		float moveSpeed;
		bool repeat;

		MovingSprite(float x, float y, Texture *texture, float speed, bool repeat, bool collideable = true, int offset = 0, bool visible = true);
		void tick(double frameTime) override;
		void addMove(sf::Vector2f move);
		sf::Vector2f getNextCoords(double frameTime);
	protected:
		virtual void move(double frameTime);
	private:
		std::queue<sf::Vector2f> moveQueue;
		sf::Vector2f diff;
		float currentLength;
		double currentMoveTime;
		sf::Vector2f currentMove;
		bool first;

	};

	struct AnimatedMovingSprite : AnimatedSprite, MovingSprite
	{
		AnimatedMovingSprite(float x, float y, Texture *texture, float moveSpeed, bool repeat, float animationSpeed, bool collideable = true, int animationOffsetChange = 0, int offset = 0, bool visible = true);
		void tick(double frameTime) override;
	};
}
