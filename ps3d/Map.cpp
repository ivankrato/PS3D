#include "Map.h"
#include "Engine.h"

extern const float COLLISION_RADIUS;

void ps3d::Map::removeFlaggedSprites()
{
	for (Sprite* removeSprite : spritesFlaggedForRemove)
	{
		sprites->erase(remove(sprites->begin(), sprites->end(), removeSprite), sprites->end());
	}
	spritesFlaggedForRemove.clear();
}

ps3d::Map::Map(sf::Vector2i size, Player *player, Texture *skybox, sf::Color floorColor, sf::Color ceilingColor, std::vector<Sprite*> *sprites) : Tickable()
{
	this->size = size;
	this->player = player;
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
		this->sprites->clear();
	}
	spritesFlaggedForRemove.clear();
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
	if (coords.x < 0 || coords.x >= size.x
		|| coords.y < 0 || coords.y >= size.y)
		return &Wall::EMPTY_WALL;
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
	spritesFlaggedForRemove.push_back(sprite);
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

sf::Vector2<bool> ps3d::Map::canMove(sf::Vector2f newPos, sf::Vector2f oldPos)
{
	sf::Vector2<bool> ret(true, true);
	//walls
	std::vector<Wall*> wallsToCheck;
	//x
	wallsToCheck.clear();
	wallsToCheck.push_back(getWall(sf::Vector2i(int(newPos.x + COLLISION_RADIUS), int(oldPos.y + COLLISION_RADIUS))));
	wallsToCheck.push_back(getWall(sf::Vector2i(int(newPos.x + COLLISION_RADIUS), int(oldPos.y - COLLISION_RADIUS))));
	wallsToCheck.push_back(getWall(sf::Vector2i(int(newPos.x + COLLISION_RADIUS), int(oldPos.y))));
	wallsToCheck.push_back(getWall(sf::Vector2i(int(newPos.x + COLLISION_RADIUS), int(oldPos.y))));
	wallsToCheck.push_back(getWall(sf::Vector2i(int(newPos.x - COLLISION_RADIUS), int(oldPos.y + COLLISION_RADIUS))));
	wallsToCheck.push_back(getWall(sf::Vector2i(int(newPos.x - COLLISION_RADIUS), int(oldPos.y - COLLISION_RADIUS))));
	wallsToCheck.push_back(getWall(sf::Vector2i(int(newPos.x - COLLISION_RADIUS), int(oldPos.y))));
	wallsToCheck.push_back(getWall(sf::Vector2i(int(newPos.x - COLLISION_RADIUS), int(oldPos.y))));
	for (Wall* wall : wallsToCheck)
	{
		if (wall != nullptr && wall->collideable)
		{
			ret.x = false;
			break;
		}
	}
	//y
	wallsToCheck.clear();
	wallsToCheck.push_back(getWall(sf::Vector2i(int(oldPos.x + COLLISION_RADIUS), int(newPos.y + COLLISION_RADIUS))));
	wallsToCheck.push_back(getWall(sf::Vector2i(int(oldPos.x + COLLISION_RADIUS), int(newPos.y - COLLISION_RADIUS))));
	wallsToCheck.push_back(getWall(sf::Vector2i(int(oldPos.x + COLLISION_RADIUS), int(newPos.y))));
	wallsToCheck.push_back(getWall(sf::Vector2i(int(oldPos.x + COLLISION_RADIUS), int(newPos.y))));
	wallsToCheck.push_back(getWall(sf::Vector2i(int(oldPos.x - COLLISION_RADIUS), int(newPos.y + COLLISION_RADIUS))));
	wallsToCheck.push_back(getWall(sf::Vector2i(int(oldPos.x - COLLISION_RADIUS), int(newPos.y - COLLISION_RADIUS))));
	wallsToCheck.push_back(getWall(sf::Vector2i(int(oldPos.x - COLLISION_RADIUS), int(newPos.y))));
	wallsToCheck.push_back(getWall(sf::Vector2i(int(oldPos.x - COLLISION_RADIUS), int(newPos.y))));
	for (Wall* wall : wallsToCheck)
	{
		if (wall != nullptr && wall->collideable)
		{
			ret.y = false;
			break;
		}
	}
	//sprites
	for (Sprite* sprite : *sprites)
	{
		if (sprite->collideable)
		{
			//x
			sf::Vector2f* spriteSquare = sprite->getCollisionSquare();
			if (newPos.x >= spriteSquare[0].x
				&& newPos.x <= spriteSquare[1].x
				&& oldPos.y >= spriteSquare[0].y
				&& oldPos.y <= spriteSquare[1].y) {
				ret.x = false;
			}
			//y
			if (newPos.y >= spriteSquare[0].y
				&& newPos.y <= spriteSquare[1].y
				&& oldPos.x >= spriteSquare[0].x
				&& oldPos.x <= spriteSquare[1].x) {
				ret.y = false;
			}
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

void ps3d::Map::setSkybox(Texture* skybox)
{
	this->skybox = skybox;
}

void ps3d::Map::setFloorColor(sf::Color floor_color)
{
	floorColor = floor_color;
}

void ps3d::Map::setCeilingColor(sf::Color ceiling_color)
{
	ceilingColor = ceiling_color;
}

bool ps3d::Map::hasSkybox() const
{
	return skybox != nullptr;
}

void ps3d::Map::tick(double frameTime)
{
	for (int i = 0; i < size.x; i++) {
		for (int j = 0; j < size.y; j++)
		{
			if (wallArray[i][j] != nullptr)
			{
				wallArray[i][j]->tick(frameTime);
			}
		}
	}
	for (Sprite* sprite : *sprites)
	{
		sprite->tick(frameTime);
		if (sprite->hasOnCollision() && Engine::isCollision(sprite, sf::Vector2f(player->x, player->y)))
		{
			sprite->onCollision();
		}
	}

	removeFlaggedSprites();
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

ps3d::MiniMap::MiniMap(sf::Vector2i size)
{
	this->colors.clear();
	this->width = size.x;
	this->height = size.y;
	minimapArray = new int*[width];
	for(int i = 0; i < width; i++)
	{
		minimapArray[i] = new int[height];
		for(int j = 0; j < height; j++)
		{
			minimapArray[i][j] = 0;
		}
	}
}

sf::Color ps3d::MiniMap::getColor(sf::Vector2i coords)
{
	int colorNumber = INT_MAX;
	if (coords.x >= 0 && coords.x < width
		&& coords.y >= 0 && coords.y < height) {
		colorNumber = minimapArray[coords.x][coords.y];
	}
	if(colors.find(colorNumber) != colors.end())
	{
		return colors[colorNumber];
	}
	return sf::Color::Transparent;
}

void ps3d::MiniMap::addColorNumber(int i, sf::Color color)
{
	colors[i] = color;
}

void ps3d::MiniMap::setColor(int color, sf::Vector2i coords)
{
	if (coords.x >= 0 && coords.x < width
		&& coords.y >= 0 && coords.y < height) {
		minimapArray[coords.x][coords.y] = color;
	}
}

sf::Vector2i ps3d::MiniMap::getSize()
{
	return sf::Vector2i(width, height);
}

