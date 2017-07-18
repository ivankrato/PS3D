#include "Texture.h"

ps3d::Texture::Texture(std::string file, bool repeated, int realWidth, int realHeight)
{
	sfTexture.loadFromFile(file);
	sfTexture.setRepeated(repeated);
	this->realWidth = realWidth > 0 ? realWidth : sfTexture.getSize().x;
	this->realHeight = realHeight > 0 ? realHeight : sfTexture.getSize().y;
}

int ps3d::Texture::getWidth()
{
	return realWidth;
}

int ps3d::Texture::getHeight()
{
	return realHeight;
}

sf::Texture* ps3d::Texture::getSfTexture()
{
	return &sfTexture;
}
