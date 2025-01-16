#pragma once

#include "Vector2.h"

class SpriteLoadException : std::exception
{
};

class Sprite
{
public:
	Sprite(const char* filename, SDL_Renderer* renderer);
	~Sprite();

	int getWidth() { return m_width; }
	int getHeight() { return m_height; }

	void draw(SDL_Renderer* renderer, const Vector2& position,
		float rotation, float rotPivotX = 0.5f, float rotPivotY = 0.5f);

private:
	SDL_Texture *m_texture = nullptr;
	int m_width, m_height;
};

