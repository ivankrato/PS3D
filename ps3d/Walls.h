#pragma once
#include "Interfaces.h"
#include "Texture.h"

namespace ps3d
{
	struct Wall : Tickable
	{
		/**
		 * \brief wall whick is not visible nor collideable
		 */
		static Wall EMPTY_WALL;

		/**
		 * \brief constructor
		 * \param texture texture of the wall
		 * \param offset texture offset
		 * \param visible true if wall should be rendered
		 * \param collideable true if the player should collide with the wall
		 */
		explicit Wall(Texture *texture, int offset = 0, bool visible = true, bool collideable = true);
		/**
		 * \brief true if wall should be rendered
		 */
		bool visible;
		/**
		 * \brief true if the player should collide with the wall
		 */
		bool collideable;
		/**
		 * \brief texture of the wall
		 */
		Texture *texture;
		/**
		 * \brief texture offset
		 */
		int offset;
		/**
		 * \brief sets offset of the next game texture in the texture file
		 * \param by if set, the offset is change by this number
		 */
		void setNextOffset(int by = 0);
	};

	struct AnimatedWall : Wall
	{
		/**
		 * \brief speed of the animation, in offset changes/second
		 */
		float speed;

		/**
		 * \brief constructor
		 * \param texture texture of the wall
		 * \param speed speed of the animation, in offset changes/second
		 * \param animationOffsetChange offset to be change, if 0 it's set to game texture width
		 * \param offset texture offset
		 * \param visible true if wall should be rendered
		 * \param collideable true if the player should collide with the wall
		 */
		AnimatedWall(Texture *texture, float speed, int animationOffsetChange = 0, int offset = 0, bool visible = true, bool collideable = true);
		/**
		 * \brief function to be called each tick
		 * \param frameTime time of a frame
		 */
		void tick(double frameTime) override;
	protected:
		/**
		 * \brief function to be called each tick to animate the wall
		 * \param frameTime time of a frame
		 */
		virtual void animate(double frameTime);
	private:
		int animationOffsetChange;
		double time;
	};
}
