#include "GameReport.h"

static const int DEFAULT_FONT_SIZE = 24;

ps3d::GameReport::GameReport()
{
	isEnd = false;
	topLeftSize = DEFAULT_FONT_SIZE;
	bottomLeftSize = DEFAULT_FONT_SIZE;
	bottomRightSize = DEFAULT_FONT_SIZE;
	endSmallText = DEFAULT_FONT_SIZE * 2;
	endSmallSize = DEFAULT_FONT_SIZE;
}
