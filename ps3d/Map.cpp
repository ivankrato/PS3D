#include "Map.h"

static const float COLLISION_RADIUS = 0.33f;

ps3d::Map::Map(sf::Vector2i size, Texture *skybox, sf::Color floorColor, sf::Color ceilingColor, std::vector<Sprite*> *sprites)
{
	this->size = size;
	wallArray = new Wall**[size.x];
	for (int i = 0; i < size.x; i++)
	{
		wallArray[i] = new Wall*[size.y];
		for (int j = 0; j < size.y; j++)
		{
			wallArray[i][j] = nullptr;
		}
	}
	this->floorColor = floorColor;
	this->ceilingColor = ceilingColor;
	this->skybox = skybox;
	this->sprites = sprites;
	if (sprites == nullptr)
	{
		this->sprites = new std::vector<Sprite*>;
		this->sprites->empty();
	}
}

sf::Vector2i ps3d::Map::getSize() const
{
	return size;
}

ps3d::Wall*** ps3d::Map::getWallArray() const
{
	return wallArray;
}

ps3d::Wall* ps3d::Map::getWall(sf::Vector2i coords) const
{
	// TODO ošetřit mimo pole
	return wallArray[coords.x][coords.y];
}

std::vector<ps3d::Sprite*>* ps3d::Map::getSprites() const
{
	return sprites;
}

void ps3d::Map::addSprite(Sprite* sprite)
{
	sprites->push_back(sprite);
}

void ps3d::Map::removeSprite(Sprite* sprite)
{
	sprites->erase(remove(sprites->begin(), sprites->end(), sprite), sprites->end());
}

ps3d::Texture* ps3d::Map::getSkybox() const
{
	return skybox;
}

void ps3d::Map::addWall(Wall* wall, sf::Vector2i coords)
{
	wallArray[coords.x][coords.y] = wall;
}

void ps3d::Map::removeWall(sf::Vector2i coords)
{
	delete wallArray[coords.x][coords.y];
	wallArray[coords.x][coords.y] = nullptr;
}

bool ps3d::Map::willCollideX(float newPosX, float oldPosY)
{
	std::vector<Wall*> wallsToCheck;
	wallsToCheck.empty();
	wallsToCheck.push_back(wallArray[int(newPosX + COLLISION_RADIUS)][int(oldPosY + COLLISION_RADIUS)]);
	wallsToCheck.push_back(wallArray[int(newPosX + COLLISION_RADIUS)][int(oldPosY - COLLISION_RADIUS)]);
	wallsToCheck.push_back(wallArray[int(newPosX + COLLISION_RADIUS)][int(oldPosY)]);
	wallsToCheck.push_back(wallArray[int(newPosX + COLLISION_RADIUS)][int(oldPosY)]);
	wallsToCheck.push_back(wallArray[int(newPosX - COLLISION_RADIUS)][int(oldPosY + COLLISION_RADIUS)]);
	wallsToCheck.push_back(wallArray[int(newPosX - COLLISION_RADIUS)][int(oldPosY - COLLISION_RADIUS)]);
	wallsToCheck.push_back(wallArray[int(newPosX - COLLISION_RADIUS)][int(oldPosY)]);
	wallsToCheck.push_back(wallArray[int(newPosX - COLLISION_RADIUS)][int(oldPosY)]);
	bool ret = false;
	for (Wall* wall : wallsToCheck)
	{
		if(wall != nullptr && wall->collideable)
		{
			ret = true;
			break;
		}
	}
	return ret;
}

bool ps3d::Map::willCollideY(float newPosY, float oldPosX)
{
	std::vector<Wall*> wallsToCheck;
	wallsToCheck.empty();
	wallsToCheck.push_back(wallArray[int(oldPosX + COLLISION_RADIUS)][int(newPosY + COLLISION_RADIUS)]);
	wallsToCheck.push_back(wallArray[int(oldPosX + COLLISION_RADIUS)][int(newPosY - COLLISION_RADIUS)]);
	wallsToCheck.push_back(wallArray[int(oldPosX + COLLISION_RADIUS)][int(newPosY)]);
	wallsToCheck.push_back(wallArray[int(oldPosX + COLLISION_RADIUS)][int(newPosY)]);
	wallsToCheck.push_back(wallArray[int(oldPosX - COLLISION_RADIUS)][int(newPosY + COLLISION_RADIUS)]);
	wallsToCheck.push_back(wallArray[int(oldPosX - COLLISION_RADIUS)][int(newPosY - COLLISION_RADIUS)]);
	wallsToCheck.push_back(wallArray[int(oldPosX - COLLISION_RADIUS)][int(newPosY)]);
	wallsToCheck.push_back(wallArray[int(oldPosX - COLLISION_RADIUS)][int(newPosY)]);
	bool ret = false;
	for (Wall* wall : wallsToCheck)
	{
		if (wall != nullptr && wall->collideable)
		{
			ret = true;
			break;
		}
	}
	return ret;
}

sf::Color ps3d::Map::getFloorColor() const
{
	return floorColor;
}

sf::Color ps3d::Map::getCeilingColor() const
{
	return ceilingColor;
}

bool ps3d::Map::hasSkybox() const
{
	return skybox != nullptr;
}

ps3d::Map::~Map()
{
	for (int i = 0; i < size.x; i++) {
		for (int j = 0; j < size.y; j++)
		{
			delete wallArray[i][j];
		}
		delete[] wallArray[i];
	}
	delete[] wallArray;
	delete skybox;
	delete sprites;
}
