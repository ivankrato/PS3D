#pragma once
#include <SFML/System.hpp>
#include "Interfaces.h"
#include "Texture.h"
#include <queue>

namespace ps3d
{
	struct Sprite : Tickable
	{
		/**
		 * \brief texture of the sprite
		 */
		Texture *texture;
		/**
		 * \brief x coordinate
		 */
		float x;
		/**
		 * \brief y coordinate
		 */
		float y;
		/**
		 * \brief distance between the sprite and the player (for rendering)
		 */
		float dist;
		/**
		 * \brief true if the sprite should be rendered
		 */
		bool visible;
		/**
		 * \brief true if the player should collide with the sprite
		 */
		bool collideable;
		/**
		 * \brief texture offset
		 */
		int offset;

		/**
		 * \brief constructor
		 * \param x x coordinate
		 * \param y y coordinate
		 * \param texture texture of the sprite
		 * \param collideable true if the player should collide with the sprite
		 * \param offset texture offset
		 * \param visible true if the sprite should be rendered
		 */
		explicit Sprite(float x, float y, Texture *texture, bool collideable = true, int offset = 0, bool visible = true);
		/**
		 * \return square where collisions happen
		 */
		sf::Vector2f* getCollisionSquare();
		/**
		* \brief sets offset of the next game texture in the texture file
		* \param by if set, the offset is change by this number
		*/
		void setNextOffset(int by = 0);
		/**
		 * \return true, if this sprite has onCollision event set
		 */
		bool hasOnCollision();
		/**
		 * \brief sets onCollision event
		 * \param onCollisionEvent function to be called on collision
		 */
		void setOnCollision(std::function<void()> onCollisionEvent);
		/**
		 * \brief triggers the onCollision event
		 */
		void onCollision();

		/**
		 * \brief compares two sprites by distance
		 * \param a first sprite
		 * \param b second sprite
		 * \return a.distance > b.distance
		 */
		static bool compareByDist(Sprite *a, Sprite *b);
	protected:
		/**
		 * \brief calculates the collision square
		 */
		void calculateCollisionSquare();
	private:
		sf::Vector2f collisionSquare[2];
		std::function<void()> onCollisionEvent;
	};

	struct AnimatedSprite : virtual Sprite
	{
		/**
		 * \brief speed of the animation, in changes/second
		 */
		float animationSpeed;

		/**
		 * \brief constructor
		 * \param x x coordinate
		 * \param y y coordinate
		 * \param texture texture of the sprite
		 * \param speed speed of the animation, in changes/second
		 * \param collideable true if the player should collide with the sprite
		 * \param animationOffsetChange offset to be change, if 0 it's set to game texture width
		 * \param offset texture offset
		 * \param visible true if the sprite should be rendered
		 */
		AnimatedSprite(float x, float y, Texture *texture, float speed, bool collideable = true, int animationOffsetChange = 0, int offset = 0, bool visible = true);
		/**
		 * \brief function to be called each tick
		 * \param frameTime time of a frame
		 */
		void tick(double frameTime) override;
	protected:
		/**
		* \brief function to be called each tick to animate the sprite
		* \param frameTime time of a frame
		*/
		virtual void animate(double frameTime);
	private:
		int animationOffsetChange;
		double time;
	};

	struct MovingSprite : virtual Sprite
	{
		/**
		 * \brief sprite moving speed, in blocks/second
		 */
		float moveSpeed;
		/**
		 * \brief true if the moveset should be repeated
		 */
		bool repeat;

		/**
		 * \brief constructor
		 * \param x x coordinate
		 * \param y y coordinate
		 * \param texture texture of the sprite
		 * \param speed sprite moving speed, in blocks/second
		 * \param repeat true if the moveset should be repeated
		 * \param collideable true if the player should collide with the sprite
		 * \param offset texture offset
		 * \param visible true if the sprite should be rendered
		 */
		MovingSprite(float x, float y, Texture *texture, float speed, bool repeat, bool collideable = true, int offset = 0, bool visible = true);
		/**
		* \brief function to be called each tick
		* \param frameTime time of a frame
		*/
		void tick(double frameTime) override;
		/**
		 * \brief adds move to the move queue
		 * \param move coordinates of the move
		 */
		void addMove(sf::Vector2f move);
		/**
		 * \param frameTime time of a frame
		 * \return coordinates of the sprite in the next tick
		 */
		sf::Vector2f getNextCoords(double frameTime);
	protected:
		/**
		 * \brief function to be called each tick to move the sprite
		 * \param frameTime time of a frame
		 */
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
		/**
		 * \brief constructor
		 * \param x x coordinate
		 * \param y y coordinate
		 * \param texture texture of the sprite
		 * \param moveSpeed sprite moving speed, in blocks/second
		 * \param repeat true if the moveset should be repeated
		 * \param animationSpeed speed of the animation, in changes/second
		 * \param collideable true if the player should collide with the sprite
		 * \param animationOffsetChange offset to be change, if 0 it's set to game texture width
		 * \param offset texture offset
		 * \param visible true if the sprite should be rendered
		 */
		AnimatedMovingSprite(float x, float y, Texture *texture, float moveSpeed, bool repeat, float animationSpeed, bool collideable = true, int animationOffsetChange = 0, int offset = 0, bool visible = true);
		/**
		* \brief function to be called each tick
		* \param frameTime time of a frame
		*/
		void tick(double frameTime) override;
	};
}
