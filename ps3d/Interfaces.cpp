#include "Interfaces.h"

ps3d::Tickable::Tickable()
{
	this->tickFunc = nullptr;
}

void ps3d::Tickable::setOnTick(std::function<void(double)> tickFunc)
{
	this->tickFunc = tickFunc;
}

void ps3d::Tickable::tick(double frameTime)
{
	if(tickFunc != nullptr)
	{
		tickFunc(frameTime);
	}
}
