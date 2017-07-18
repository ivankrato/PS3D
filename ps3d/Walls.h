#pragma once

namespace ps3d
{
	class IGame;
	class Texture;

	struct Wall
	{
		explicit Wall(Texture *texture, bool visible = true, bool collideable = true);
		bool visible;
		bool collideable;
		Texture *texture;
	};

	struct EventWall : public Wall
	{
		explicit EventWall(Texture *texture, bool visible = true, bool collideable = true);
		virtual void Event(IGame *game) = 0;
	};
	
}
