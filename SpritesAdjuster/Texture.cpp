#include "Texture.h"

LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}

bool LTexture::loadFromFile(std::string path)
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Color key image
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

#ifdef _SDL_TTF_H
bool LTexture::loadFromRenderedText(std::string textureText, SDL_Color textColor)
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);
	if (textSurface == NULL)
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		//Create texture from surface pixels
		mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
		if (mTexture == NULL)
		{
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}

	//Return success
	return mTexture != NULL;
}
#endif

void LTexture::free()
{
	//Free texture if it exists
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
	//Modulate texture rgb
	SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void LTexture::setBlendMode(SDL_BlendMode blending)
{
	//Set blending function
	SDL_SetTextureBlendMode(mTexture, blending);
}

void LTexture::setAlpha(Uint8 alpha)
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod(mTexture, alpha);
}

void LTexture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	int x0, x1, y0, y1;
	int mx0, mx1, my0, my1;

	double angleRad = angle * 3.1415 / 180.0;
	if (center != nullptr)
	{
		int xCen = mWidth / 2;
		int yCen = mHeight / 2;

		//x0 = center->x - xCen;
		//x1 = center->x + mWidth - xCen;
		//y0 = center->y - yCen;
		//y1 = center->y + mHeight - yCen;

		x0 = -xCen;
		x1 = xCen;
		y0 = -yCen;
		y1 = yCen;



		//mx0 = y0 * cos(angleRad) - x0 * sin(angleRad);
		//mx1 = y1 * cos(angleRad) - x1 * sin(angleRad);
		//my0 = y0 * sin(angleRad) + x0 * cos(angleRad);
		//my1 = y1 * sin(angleRad) + x1 * cos(angleRad);

		mx0 = x0 * cos(angleRad) - y0 * sin(angleRad);
		mx1 = x1 * cos(angleRad) - y1 * sin(angleRad);
		my0 = y0 * cos(angleRad) + x0 * sin(angleRad);
		my1 = y1 * cos(angleRad) + x1 * sin(angleRad);


		SDL_Point P0, P1, P2, P3, P4;
		SDL_Point mP0, mP1, mP2, mP3, mP4;

		P0 = { x0, y0 };
		P1 = { x1, y0 };
		P2 = { x1, y1 };
		P3 = { x0, y1 };
		P4 = { x0, y0 };


		mP0 = { x + mx0 + mWidth / 2,y + my0 + mHeight / 2 };
		mP1 = { x + mx1 + mWidth / 2,y + my0 + mHeight / 2 };
		mP2 = { x + mx1 + mWidth / 2,y + my1 + mHeight / 2 };
		mP3 = { x + mx0 + mWidth / 2,y + my1 + mHeight / 2 };
		mP4 = { x + mx0 + mWidth / 2,y + my0 + mHeight/2 };

		SDL_Point points[5] = { P0, P1, P2, P3, P4 };
		SDL_Point mPoints[5] = { mP0, mP1, mP2, mP3, mP4 };

		//Render to screen
		
		if (true)
		{
			SDL_SetRenderDrawColor(gRenderer, 0xFF, 0, 0xFF, 0xFF);
			SDL_RenderDrawLines(gRenderer, mPoints, 5);
			//SDL_RenderDrawRect(gRenderer, &renderQuad);
		}
	}
	SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

void LTexture::setRenderer(SDL_Renderer * gRenderer)
{
	this->gRenderer = gRenderer;
}

void LTexture::setRenderRectsFlag()
{
	mRenderRects = !mRenderRects;
}
