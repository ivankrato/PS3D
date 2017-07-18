#include "Engine.h"
#include "Maze3D/Maze3D.h"

int main()
{
	Maze3D::Game game;
	ps3d::Engine engine(&game);
	engine.start();
	return 0;
}
