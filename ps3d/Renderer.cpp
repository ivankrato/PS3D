#include "Renderer.h"
#include "Texture.h"

extern const float DEFAULT_PLANE_WIDTH;
static const float FOV = 2 * atan(DEFAULT_PLANE_WIDTH / 1.0f);
static const float PI = 3.14159265359f;
static const float DRAW_DISTANCE = 200;

static const float WALL_HEIGHT_MULTIPLIER = 1.33f;

static const char* FONT_FILE = "Resources/NixieOne.ttf";

void ps3d::Renderer::renderBG()
{
	window->clear(map->getFloorColor()); //floor
	if (map->hasSkybox())
	{
		Texture *skybox = map->getSkybox();
		sf::Sprite skyboxSprite;
		skyboxSprite.setTexture(*skybox->getSfTexture());
		float angle = atan2(player->getDirX(), player->getDirY());
		float skyboxTexWidth = float(skybox->getWidth());
		float skyboxWidth = FOV / (2 * PI)*skyboxTexWidth;
		float skyboxTexHeight = float(skybox->getHeight());
		float scaleY = window->getSize().y / skyboxTexHeight;
		float scaleX = window->getSize().x / skyboxWidth;
		float texX = skyboxTexWidth*(angle / (2 * PI));
		skyboxSprite.setTextureRect(sf::IntRect(int(texX), 0, int(skyboxWidth), int(skyboxTexHeight / 2)));
		skyboxSprite.setScale(sf::Vector2f(scaleX, scaleY));
		window->draw(skyboxSprite);
	}
	else
	{
		sf::RectangleShape ceilingRect(sf::Vector2f(float(window->getSize().x), float(window->getSize().y / 2)));
		ceilingRect.setFillColor(map->getCeilingColor());
		window->draw(ceilingRect);
	}
}

void ps3d::Renderer::renderWallsSprites()
{

	int w = window->getSize().x;
	int h = window->getSize().y;

	float *zBuffer = new float[w];

	sf::VertexArray line(sf::Lines, 2);
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
		float perpWallDist = 0;

		//what direction to step in x or y-direction (either +1 or -1)
		sf::Vector2i step;

		bool hit = false; //was there a wall hit?
		bool visible = false;
		int side = 0; //was a NS or a EW wall hit?
				  //calculate step and initial sideDist
		if (rayDir.x < 0)
		{
			step.x = -1;
			sideDist.x = (rayPos.x - mapCoords.x) * deltaDist.x;
		}
		else
		{
			step.x = 1;
			sideDist.x = (mapCoords.x + 1.0f - rayPos.x) * deltaDist.x;
		}
		if (rayDir.y < 0)
		{
			step.y = -1;
			sideDist.y = (rayPos.y - mapCoords.y) * deltaDist.y;
		}
		else
		{
			step.y = 1;
			sideDist.y = (mapCoords.y + 1.0f - rayPos.y) * deltaDist.y;
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
			//Calculate distance projected on camera direction (oblique distance will give fisheye effect!)
			if (side == 0) perpWallDist = (mapCoords.x - rayPos.x + (1 - step.x) / 2) / rayDir.x;
			else           perpWallDist = (mapCoords.y - rayPos.y + (1 - step.y) / 2) / rayDir.y;
			if (perpWallDist > DRAW_DISTANCE)
			{
				wall = &Wall::EMPTY_WALL;
				hit = true;
			}
		}
		if (wall->visible)
		{

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
			line[0].position = sf::Vector2f(float(x), float(drawStart));
			line[1].position = sf::Vector2f(float(x), float(drawEnd));
			int realTexX = texX + wall->offset;
			line[0].texCoords = sf::Vector2f(float(realTexX), 0.0f);
			line[1].texCoords = sf::Vector2f(float(realTexX), float(texWidth));
			window->draw(line, wall->texture->getSfTexture());

			zBuffer[x] = perpWallDist;
		} else
		{
			zBuffer[x] = std::numeric_limits<float>::max();
		}
	}
	//SPRITES
	std::vector<Sprite*> *sprites = map->getSprites();
	for (Sprite *sprite : *sprites)
	{
		sprite->dist = (player->x - sprite->x) * (player->x - sprite->x) + (player->y - sprite->y) * (player->y - sprite->y);
	}
	sort(sprites->begin(), sprites->end(), Sprite::compareByDist);
	//after sorting the sprites, do the projection and draw them
	float invDet = 1.0f / (player->getPlaneX() * player->getDirY() - player->getDirX() * player->getPlaneY()); //required for correct matrix multiplication
	for (int i = 0; i < sprites->size(); i++)
	{
		Sprite *sprite = sprites->at(i);
		if (sprite->visible)
		{
			//translate sprite position to relative to camera
			sf::Vector2f relSprite;
			relSprite.x = sprite->x - player->x;
			relSprite.y = sprite->y - player->y;

			//transform sprite with the inverse camera matrix
			// [ planeX   dirX ] -1                                       [ dirY      -dirX ]
			// [               ]       =  1/(planeX*dirY-dirX*planeY) *   [                 ]
			// [ planeY   dirY ]                                          [ -planeY  planeX ]

			sf::Vector2f spriteTransform;
			spriteTransform.x = invDet * (player->getDirY() * relSprite.x - player->getDirX() * relSprite.y);
			spriteTransform.y = invDet * (-player->getPlaneY() * relSprite.x + player->getPlaneX() * relSprite.y); //this is actually the depth inside the screen, that what Z is in 3D

			int spriteScreenX = int((w / 2) * (1 + spriteTransform.x / spriteTransform.y));

			//calculate height of the sprite on screen
			int spriteHeight = abs(int(h * WALL_HEIGHT_MULTIPLIER / spriteTransform.y)); //using "transformY" instead of the real distance prevents fisheye

																						 //calculate lowest and highest pixel to fill in current stripe
			sf::Vector2i drawStart;
			sf::Vector2i drawEnd;
			drawStart.y = -spriteHeight / 2 + h / 2;
			drawEnd.y = spriteHeight / 2 + h / 2;

			//calculate width of the sprite
			int spriteWidth = abs(int(h * WALL_HEIGHT_MULTIPLIER / spriteTransform.y));
			drawStart.x = -spriteWidth / 2 + spriteScreenX;
			if (drawStart.x < 0) drawStart.x = 0;
			drawEnd.x = spriteWidth / 2 + spriteScreenX;
			if (drawEnd.x >= w) drawEnd.x = w - 1;

			//loop through every vertical stripe of the sprite on screen
			Texture *spriteTexture = sprite->texture;
			int texWidth = spriteTexture->getWidth();
			for (int stripe = drawStart.x; stripe < drawEnd.x; stripe++)
			{
				int texX = int(256 * (stripe - (-spriteWidth / 2 + spriteScreenX)) * texWidth / spriteWidth) / 256;
				//the conditions in the if are:
				//1) it's in front of camera plane so you don't see things behind you
				//2) it's on the screen (left)
				//3) it's on the screen (right)
				//4) ZBuffer, with perpendicular distance
				if (spriteTransform.y > 0 && stripe > 0 && stripe < w && spriteTransform.y < zBuffer[stripe]) {
					//draw line
					line[0].position = sf::Vector2f(float(stripe), float(drawStart.y));
					line[1].position = sf::Vector2f(float(stripe), float(drawEnd.y));
					int realTexX = texX + sprite->offset;
					line[0].texCoords = sf::Vector2f(float(realTexX), 0.0f);
					line[1].texCoords = sf::Vector2f(float(realTexX), float(texWidth));
					window->draw(line, spriteTexture->getSfTexture());
				}
			}
		}
	}

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
	this->font = new sf::Font;
	this->font->loadFromFile(FONT_FILE);
}

void ps3d::Renderer::render(int fps)
{
	renderBG();
	renderWallsSprites();
	renderHUD();
	if (fps > 0) drawFPS(fps);

	window->display();
}

void ps3d::Renderer::drawFPS(int fps)
{
	sf::Text text;
	text.setFont(*font);
	text.setString(std::to_string(fps));
	text.setCharacterSize(24); // in pixels, not points!
	text.setFillColor(sf::Color::White);
	text.setStyle(sf::Text::Bold);
	window->draw(text);
}
