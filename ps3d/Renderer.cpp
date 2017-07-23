#include "Renderer.h"
#include "Texture.h"
#include <iostream>
#include <thread>
#include "Settings.h"

extern const float DEFAULT_PLANE_WIDTH;
static const float FOV = 2 * atan(DEFAULT_PLANE_WIDTH / 1.0f);
static const float PI = 3.14159265359f;
static const int DEFAULT_DRAW_DISTANCE = 200;

static const float DEFAULT_WALL_HEIGHT_MULTIPLIER = 1.33f;

static const char* FONT_FILE = "Resources/NixieOne.ttf";
static const float TEXT_MARGIN_X = 10;
static const float TEXT_MARGIN_Y = 10;

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
			if (perpWallDist > drawDistance)
			{
				wall = &Wall::EMPTY_WALL;
				hit = true;
			}
		}
		if (wall->visible)
		{

			//Calculate height of line to draw on screen
			int lineHeight = int(h * wallHeightMultiplier / perpWallDist);

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

			//draw the pixels of the stripe as a vertical line
			line[0].position = sf::Vector2f(float(x), float(drawStart));
			line[1].position = sf::Vector2f(float(x), float(drawEnd));
			int realTexX = (texX + wall->offset) % wall->texture->getSfTexture()->getSize().x;
			line[0].texCoords = sf::Vector2f(float(realTexX), 0.0f);
			line[1].texCoords = sf::Vector2f(float(realTexX), float(texWidth));
			window->draw(line, wall->texture->getSfTexture());

			zBuffer[x] = perpWallDist;
		}
		else
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
			int spriteHeight = abs(int(h * wallHeightMultiplier / spriteTransform.y)); //using "transformY" instead of the real distance prevents fisheye

																						 //calculate lowest and highest pixel to fill in current stripe
			sf::Vector2i drawStart;
			sf::Vector2i drawEnd;
			drawStart.y = -spriteHeight / 2 + h / 2;
			drawEnd.y = spriteHeight / 2 + h / 2;

			//calculate width of the sprite
			int spriteWidth = abs(int(h * wallHeightMultiplier / spriteTransform.y));
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
					int realTexX = (texX + sprite->offset) % spriteTexture->getSfTexture()->getSize().x;
					line[0].texCoords = sf::Vector2f(float(realTexX), 0.0f);
					line[1].texCoords = sf::Vector2f(float(realTexX), float(texWidth));
					window->draw(line, spriteTexture->getSfTexture());
				}
			}
		}
	}

	delete[] zBuffer;
}

void ps3d::Renderer::renderHUD(GameReport &gameReport)
{
	// miniMap
	sf::Image miniMapImage;
	sf::Vector2i miniMapSize = miniMap->getSize();
	sf::Uint8 *pixelArray = new sf::Uint8[miniMapSize.x*miniMapSize.y * 4];
	for (int i = 0; i < miniMapSize.x; i++)
	{
		for (int j = 0; j < miniMapSize.y; j++) {
			sf::Color color = miniMap->getColor(sf::Vector2i(i, j));
			pixelArray[(j + i * miniMapSize.y) * 4] = color.r;
			pixelArray[(j + i * miniMapSize.y) * 4 + 1] = color.g;
			pixelArray[(j + i * miniMapSize.y) * 4 + 2] = color.b;
			pixelArray[(j + i * miniMapSize.y) * 4 + 3] = color.a;
		}
	}
	miniMapImage.create(miniMapSize.y, miniMapSize.x, pixelArray);
	miniMapTexture->loadFromImage(miniMapImage);
	miniMapSprite->setTexture(*miniMapTexture);
	// miniMap size
	float scale = window->getSize().y / 4.0f / miniMapSize.y;
	miniMapSprite->setPosition(float(window->getSize().x), 0);
	miniMapSprite->setRotation(90.f);
	miniMapSprite->setScale(scale, scale);
	window->draw(*miniMapSprite);

	// other
	sf::FloatRect boundingBox;
	// top left text
	if (!gameReport.topLeftText.empty()) {
		textHUD->setOrigin(0, 0);
		textHUD->setPosition(TEXT_MARGIN_X, TEXT_MARGIN_Y);
		textHUD->setCharacterSize(gameReport.topLeftSize);
		textHUD->setString(gameReport.topLeftText);
		textHUD->setPosition(TEXT_MARGIN_X, TEXT_MARGIN_Y);
		window->draw(*textHUD);
	}
	// bottom left text
	if (!gameReport.bottomLeftText.empty()) {
		textHUD->setCharacterSize(gameReport.bottomLeftSize);
		textHUD->setString(gameReport.bottomLeftText);
		boundingBox = textHUD->getLocalBounds();
		textHUD->setOrigin(0, boundingBox.height + boundingBox.top);
		textHUD->setPosition(TEXT_MARGIN_X, float(window->getSize().y) - TEXT_MARGIN_Y);
		window->draw(*textHUD);
	}
	// bottom right text
	if (!gameReport.bottomRightText.empty()) {
		textHUD->setCharacterSize(gameReport.bottomRightSize);
		textHUD->setString(gameReport.bottomRightText);
		boundingBox = textHUD->getLocalBounds();
		textHUD->setOrigin(boundingBox.width + boundingBox.left, boundingBox.height + boundingBox.top);
		textHUD->setPosition(float(window->getSize().x) - TEXT_MARGIN_X, float(window->getSize().y) - TEXT_MARGIN_Y);
		window->draw(*textHUD);
	}

}

void ps3d::Renderer::renderEnd(GameReport& gameReport)
{
	sf::FloatRect boundingBox;
	window->clear(gameReport.endColor);
	if(!gameReport.endBigText.empty())
	{
		textHUD->setCharacterSize(gameReport.endBigSize);
		textHUD->setString(gameReport.endBigText);
		boundingBox = textHUD->getLocalBounds();
		textHUD->setOrigin((boundingBox.width + boundingBox.left) / 2.0f, boundingBox.height + boundingBox.top);
		textHUD->setPosition(window->getSize().x / 2.0f, window->getSize().y / 2.0f);
		window->draw(*textHUD);
	}
	if (!gameReport.endSmallText.empty())
	{
		textHUD->setCharacterSize(gameReport.endSmallSize);
		textHUD->setString(gameReport.endSmallText);
		boundingBox = textHUD->getLocalBounds();
		textHUD->setOrigin((boundingBox.width + boundingBox.left) / 2.0f, 0);
		textHUD->setPosition(window->getSize().x / 2.0f, window->getSize().y / 2.0f);
		window->draw(*textHUD);
	}
}

ps3d::Renderer::Renderer(sf::RenderWindow *window, Map *map, Player *player, MiniMap *miniMap, Settings *settings)
{
	this->window = window;
	this->map = map;
	this->player = player;
	this->miniMap = miniMap;
	this->font = new sf::Font;
	this->font->loadFromFile(FONT_FILE);
	this->drawDistance = settings->has("DRAW_DISTANCE") ? stoi(settings->get("DRAW_DISTANCE")) : DEFAULT_DRAW_DISTANCE;
	this->wallHeightMultiplier = settings->has("WALL_HEIGHT_MULTIPLIER") ? stof(settings->get("WALL_HEIGHT_MULTIPLIER")) : DEFAULT_WALL_HEIGHT_MULTIPLIER;
	this->textHUD = new sf::Text;
	this->textHUD->setFont(*font);
	this->textHUD->setFillColor(sf::Color::White);
	this->textHUD->setStyle(sf::Text::Bold);
	this->textHUD->setOutlineColor(sf::Color::Black);
	this->textHUD->setOutlineThickness(1);
	this->miniMapSprite = new sf::Sprite;
	this->miniMapTexture = new sf::Texture;
}

void ps3d::Renderer::render(GameReport &gameReport)
{
	if(!gameReport.isEnd)
	{
		renderBG();
		renderWallsSprites();
		renderHUD(gameReport);
	}
	else
	{
		renderEnd(gameReport);
	}

	window->display();
}