#include "Renderer.h"
#include <functional>

extern const float DEFAULT_PLANE_WIDTH;
static const float FOV = 2 * atan(DEFAULT_PLANE_WIDTH / 1.0f);
static const float PI = 3.14159265359f;

static const float WALL_HEIGHT_MULTIPLIER = 1.33f;

void ps3d::Renderer::renderBG()
{
	window->clear(map->getFloorColor()); //floor
	if (map->hasSkybox())
	{
		Texture *skybox = map->getSkybox();
		sf::Sprite skyboxSprite;
		skyboxSprite.setTexture(*skybox->getSfTexture());
		float angle = atan2(player->getDirX(), player->getDirY());
		float skyboxTexWidth = skybox->getWidth();
		float skyboxWidth = FOV / (2 * PI)*skyboxTexWidth;
		float skyboxTexHeight = skybox->getHeight();
		float scaleY = window->getSize().y / skyboxTexHeight;
		float scaleX = window->getSize().x / skyboxWidth;
		float texX = skyboxTexWidth*(angle / (2 * PI));
		skyboxSprite.setTextureRect(sf::IntRect(int(texX), 0, skyboxWidth, skyboxTexHeight / 2));
		skyboxSprite.setScale(sf::Vector2f(scaleX, scaleY));
		window->draw(skyboxSprite);
	}
	else
	{
		// TODO draw ceiling
	}
}

void ps3d::Renderer::renderWallsSprites()
{

	int w = window->getSize().x;
	int h = window->getSize().y;

	float *zBuffer = new float[w];

	for (int x = 0; x < w; x++)
	{
		//calculate ray position and direction
		float cameraX = 2 * x / float(w) - 1; //x-coordinate in camera space
		sf::Vector2f rayPos(player->x, player->y);
		sf::Vector2f rayDir;
		rayDir.x = player->getDirX() + player->getPlaneX() * cameraX;
		rayDir.y = player->getDirY() + player->getPlaneY() * cameraX;
		//which box of the map we're in
		sf::Vector2i mapCoords(int(rayPos.x), int(rayPos.y));
		//length of ray from current position to next x or y-side
		sf::Vector2f sideDist;

		//length of ray from one x or y-side to next x or y-side
		sf::Vector2f deltaDist;
		deltaDist.x = sqrt(1 + (rayDir.y * rayDir.y) / (rayDir.x * rayDir.x));
		deltaDist.y = sqrt(1 + (rayDir.x * rayDir.x) / (rayDir.y * rayDir.y));
		float perpWallDist;

		//what direction to step in x or y-direction (either +1 or -1)
		sf::Vector2i step;

		bool hit = false; //was there a wall hit?
		int side; //was a NS or a EW wall hit?
				  //calculate step and initial sideDist
		if (rayDir.x < 0)
		{
			step.x = -1;
			sideDist.x = (rayPos.x - mapCoords.x) * deltaDist.x;
		}
		else
		{
			step.x = 1;
			sideDist.x = (mapCoords.x + 1.0 - rayPos.x) * deltaDist.x;
		}
		if (rayDir.y < 0)
		{
			step.y = -1;
			sideDist.y = (rayPos.y - mapCoords.y) * deltaDist.y;
		}
		else
		{
			step.y = 1;
			sideDist.y = (mapCoords.y + 1.0 - rayPos.y) * deltaDist.y;
		}
		//perform DDA
		Wall *wall = nullptr;
		while (hit == 0)
		{
			//jump to next map square, OR in x-direction, OR in y-direction
			if (sideDist.x < sideDist.y)
			{
				sideDist.x += deltaDist.x;
				mapCoords.x += step.x;
				side = 0;
			}
			else
			{
				sideDist.y += deltaDist.y;
				mapCoords.y += step.y;
				side = 1;
			}
			//Check if ray has hit a wall
			wall = map->getWall(mapCoords);
			hit = wall != nullptr && wall->visible;
		}
		//Calculate distance projected on camera direction (oblique distance will give fisheye effect!)
		if (side == 0) perpWallDist = (mapCoords.x - rayPos.x + (1 - step.x) / 2) / rayDir.x;
		else           perpWallDist = (mapCoords.y - rayPos.y + (1 - step.y) / 2) / rayDir.y;

		//Calculate height of line to draw on screen
		int lineHeight = int(h * WALL_HEIGHT_MULTIPLIER / perpWallDist);

		//calculate lowest and highest pixel to fill in current stripe
		int drawStart = -lineHeight / 2 + h / 2;
		int drawEnd = lineHeight / 2 + h / 2;

		//calculate value of wallX
		float wallX; //where exactly the wall was hit
		if (side == 0) wallX = rayPos.y + perpWallDist * rayDir.y;
		else           wallX = rayPos.x + perpWallDist * rayDir.x;
		wallX -= floor(wallX);

		//x coordinate on the texture
		int texWidth = wall->texture->getWidth();
		int texX = int(wallX * float(texWidth));
		if (side == 0 && rayDir.x > 0 || side == 1 && rayDir.y < 0) texX = texWidth - texX - 1;
		if (texX <= 0) {
			texX = 1;
		}
		//give x and y sides different brightness TODO
		//if (side == 1) { color = sf::Color(color.toInteger()/2); }

		//draw the pixels of the stripe as a vertical line
		sf::VertexArray line(sf::Lines, 2);
		line[0].position = sf::Vector2f(x, drawStart);
		line[1].position = sf::Vector2f(x, drawEnd);
		line[0].texCoords = sf::Vector2f(texX, 0);
		line[1].texCoords = sf::Vector2f(texX, texWidth);
		window->draw(line, wall->texture->getSfTexture());

		zBuffer[x] = perpWallDist;
	}
	/* TODO
	//SPRITES
	for (auto &sprite : sprites)
	{
		sprite.distance = (posX - sprite.x) * (posX - sprite.x) + (posY - sprite.y) * (posY - sprite.y);
	}
	std::sort(sprites.begin(), sprites.end(), std::greater<Sprite>());
	//after sorting the sprites, do the projection and draw them
	for (int i = 0; i < sprites.size(); i++)
	{
		//translate sprite position to relative to camera
		float spriteX = sprites[i].x - posX;
		float spriteY = sprites[i].y - posY;

		//transform sprite with the inverse camera matrix
		// [ planeX   dirX ] -1                                       [ dirY      -dirX ]
		// [               ]       =  1/(planeX*dirY-dirX*planeY) *   [                 ]
		// [ planeY   dirY ]                                          [ -planeY  planeX ]

		float invDet = 1.0 / (planeX * dirY - dirX * planeY); //required for correct matrix multiplication

		float transformX = invDet * (dirY * spriteX - dirX * spriteY);
		float transformY = invDet * (-planeY * spriteX + planeX * spriteY); //this is actually the depth inside the screen, that what Z is in 3D

		int spriteScreenX = int((w / 2) * (1 + transformX / transformY));

		//calculate height of the sprite on screen
		int spriteHeight = abs(int(h * sizeMultiplier / transformY)); //using "transformY" instead of the real distance prevents fisheye
																	  //calculate lowest and highest pixel to fill in current stripe
		int drawStartY = -spriteHeight / 2 + h / 2;
		int drawEndY = spriteHeight / 2 + h / 2;

		//calculate width of the sprite
		int spriteWidth = abs(int(h * sizeMultiplier / (transformY)));
		int drawStartX = -spriteWidth / 2 + spriteScreenX;
		if (drawStartX < 0) drawStartX = 0;
		int drawEndX = spriteWidth / 2 + spriteScreenX;
		if (drawEndX >= w) drawEndX = w - 1;

		//loop through every vertical stripe of the sprite on screen
		for (int stripe = drawStartX; stripe < drawEndX; stripe++)
		{
			int texX = int(256 * (stripe - (-spriteWidth / 2 + spriteScreenX)) * texWidth / spriteWidth) / 256;
			//the conditions in the if are:
			//1) it's in front of camera plane so you don't see things behind you
			//2) it's on the screen (left)
			//3) it's on the screen (right)
			//4) ZBuffer, with perpendicular distance
			if (transformY > 0 && stripe > 0 && stripe < w && transformY < zBuffer[stripe]) {
				//draw line
				sf::VertexArray line(sf::Lines, 2);
				line[0].position = sf::Vector2f(stripe, drawStartY);
				line[1].position = sf::Vector2f(stripe, drawEndY);
				float texRealX = texX + (texWidth * sprites[i].texture);
				line[0].texCoords = sf::Vector2f(texRealX, 0);
				line[1].texCoords = sf::Vector2f(texRealX, texWidth);
				window.draw(line, &texture);
			}
		}
	}
	*/

	delete[] zBuffer;
}

void ps3d::Renderer::renderHUD()
{
}

ps3d::Renderer::Renderer(sf::RenderWindow *window, Map *map, Player *player)
{
	this->window = window;
	this->map = map;
	this->player = player;
}

void ps3d::Renderer::render()
{
	renderBG();
	renderWallsSprites();
	renderHUD();

	window->display();
}
