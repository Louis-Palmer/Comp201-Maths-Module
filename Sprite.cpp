#include "stdafx.h"
#include "Sprite.h"

// Load a texture
SDL_Texture* loadTexture(const std::string& path, SDL_Renderer* renderer)
{
	// Load the surface from the specified image file
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (!loadedSurface)
	{
		std::cout << "IMG_Load(" << path << ") error: " << IMG_GetError() << std::endl;
		return nullptr;
	}
	else
	{
		//Create texture from surface pixels
		SDL_Texture* newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		if (!newTexture)
		{
			std::cout << "SDL_CreateTextureFromSurface error: " << SDL_GetError() << std::endl;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);

		return newTexture;
	}
}

Sprite::Sprite(const char* filename, SDL_Renderer* renderer)
{
	m_texture = loadTexture(filename, renderer);

	if (!m_texture)
		throw SpriteLoadException();

	// Get size
	SDL_QueryTexture(m_texture, nullptr, nullptr, &m_width, &m_height);
}

Sprite::~Sprite()
{
	if (m_texture)
	{
		SDL_DestroyTexture(m_texture);
		m_texture = nullptr;
	}
}

void Sprite::draw(SDL_Renderer* renderer, const Vector2& position, float rotation, float rotPivotX, float rotPivotY)
{
	SDL_FRect rect;
	rect.x = position.x - m_width / 2;
	rect.y = position.y - m_height / 2;
	rect.w = m_width;
	rect.h = m_height;

	SDL_FPoint pivot;
	pivot.x = m_width * rotPivotX;
	pivot.y = m_height * rotPivotY;

	SDL_RenderCopyExF(renderer, m_texture, nullptr, &rect, rotation, &pivot, SDL_RendererFlip::SDL_FLIP_NONE);
}
