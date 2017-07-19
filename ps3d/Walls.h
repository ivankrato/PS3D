#pragma once
#include "Interfaces.h"
#include "Texture.h"

namespace ps3d
{
	struct Wall : Tickable
	{
		static Wall EMPTY_WALL;

		explicit Wall(Texture *texture, int offset = 0, bool visible = true, bool collideable = true);
		bool visible;
		bool collideable;
		Texture *texture;
		int offset;
		void setNextOffset(int by = 0);
	};

	struct EventWall : public Wall
	{
		explicit EventWall(Texture *texture, bool visible = true, bool collideable = true);
		virtual void Event(IGame *game) = 0;
	};
	
}
