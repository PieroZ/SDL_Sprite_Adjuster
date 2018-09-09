//Using SDL and standard IO

#include "MouseButton.h"
#include "Texture.h"
#include <stdio.h>
#include <string>
#include <iostream>
#include <algorithm>

//Screen dimension constants
const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;

//Button constants
const int BUTTON_WIDTH = 300;
const int BUTTON_HEIGHT = 200;
const int TOTAL_BUTTONS = 4;

Uint8 gGreyness = 0x20;
Uint8 gShadowAlpha = 0x7F;

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Mouse button sprites
SDL_Rect gSpriteClips[4]; // BUTTON_SPRITE_TOTAL



LTexture gButtonSpriteSheetTexture;
LTexture gCharacterTexture;
LTexture gShadowTexture;
LTexture gBackgroundTexture;

std::string gCharacterFilepath;
std::string gBackgroundFilepath;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();


//Buttons objects
LButton gButtons[TOTAL_BUTTONS];


bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
			}
		}
	}

	gButtonSpriteSheetTexture.setRenderer(gRenderer);
	gCharacterTexture.setRenderer(gRenderer);
	gBackgroundTexture.setRenderer(gRenderer);
	gShadowTexture.setRenderer(gRenderer);

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	if (!gCharacterTexture.loadFromFile(gCharacterFilepath))
	{
		printf("Failed to load character sprite texture!\n");
		success = false;
	}

	if (!gShadowTexture.loadFromFile(gCharacterFilepath))
	{
		printf("Failed to load shadow sprite texture!\n");
		success = false;
	}
	else
	{
		gShadowTexture.setAlpha(gShadowAlpha);
		gShadowTexture.setColor(0x00, 0x00, 0x00);
		//gShadowTexture.setRenderRectsFlag();
	}

	if (!gBackgroundTexture.loadFromFile(gBackgroundFilepath))
	{
		printf("Failed to load background sprite texture!\n");
		success = false;
	}


	//Load sprites
	if (!gButtonSpriteSheetTexture.loadFromFile("button.png"))
	{
		printf("Failed to load button sprite texture!\n");
		success = false;
	}
	else
	{
		//Set sprites
		for (int i = 0; i < BUTTON_SPRITE_TOTAL; ++i)
		{
			gSpriteClips[i].x = 0;
			gSpriteClips[i].y = i * 200;
			gSpriteClips[i].w = BUTTON_WIDTH;
			gSpriteClips[i].h = BUTTON_HEIGHT;
		}

		//Set buttons in corners
		gButtons[0].setPosition(0, 0);
		gButtons[1].setPosition(SCREEN_WIDTH - BUTTON_WIDTH, 0);
		gButtons[2].setPosition(0, SCREEN_HEIGHT - BUTTON_HEIGHT);
		gButtons[3].setPosition(SCREEN_WIDTH - BUTTON_WIDTH, SCREEN_HEIGHT - BUTTON_HEIGHT);
	}

	return success;
}

void close()
{
	//Free loaded images
	gButtonSpriteSheetTexture.free();
	gCharacterTexture.free();
	gShadowTexture.free();
	gBackgroundTexture.free();

	//Destroy window	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

int main(int argc, char* args[])
{
	gCharacterFilepath = "tir.png";
	gBackgroundFilepath = "background2.png";

	int xShadowOffset = 0;
	int yShadowOffset = 0;
	int widthCut = 0;
	int heightCut = 0;
	int xRotate = 0;
	int yRotate = 0;
	double angle = 0.0;

	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		for (int i = 0; i < 4; ++i)
		{
			gButtons[i].setButtonWidth(BUTTON_WIDTH);
			gButtons[i].setButtonHeight(BUTTON_HEIGHT);
		}
		//Load media
		if (!loadMedia())
		{
			printf("Failed to load media!\n");
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//While application is running
			while (!quit)
			{
				//Handle events on queue
				while (SDL_PollEvent(&e) != 0)
				{
					//User requests quit
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}
					if (e.type == SDL_KEYUP)
					{
						switch (e.key.keysym.sym)
						{

						case SDLK_UP:
							//gCharacterTexture.setRenderRectsFlag();
							break;

						case SDLK_DOWN:
							//gShadowTexture.setAlpha(gShadowAlpha= gShadowAlpha+30);
							std::cout << " shadow Alpha = " << (int)gShadowAlpha << std::endl;
							break;

						case SDLK_LEFT:
							break;

						case SDLK_RIGHT:
							break;
						case SDLK_ESCAPE:
							quit = true;
							break;

						default:
							break;
						}
					}

					switch (e.key.keysym.sym)
					{
					case SDLK_c:
						gShadowTexture.setAlpha(gShadowAlpha++);
						break;
					case SDLK_v:
						gGreyness++;
						break;
					case SDLK_LEFT:
					case SDLK_a:
						xShadowOffset -= 1;
						std::cout << xShadowOffset << std::endl;
						break;
					case SDLK_RIGHT:
					case SDLK_d:
						xShadowOffset++;
						break;
					case SDLK_UP:
					case SDLK_w:
						yShadowOffset -= 1;
						break;

					case SDLK_DOWN:
					case SDLK_s:
						yShadowOffset++;
						break;
					case SDLK_j:
						widthCut++;
						break;
					case SDLK_k:
						heightCut++;
						break;
					case SDLK_u:
						widthCut--;
						break;
					case SDLK_i:
						heightCut--;
						break;

					case SDLK_n:
						heightCut = 0;
						widthCut = 0;
						break;

					case SDLK_1:
						xRotate--;
						break;

					case SDLK_2:
						yRotate--;
						break;

					case SDLK_4:
						xRotate++;
						break;

					case SDLK_5:
						yRotate++;
						break;
					}




					//Handle button events
					//for (int i = 0; i < TOTAL_BUTTONS; ++i)
					//{
					//	gButtons[i].handleEvent(&e);
					//}
				}

				//Clear screen
				//SDL_SetRenderDrawColor(gRenderer, gGreyness, gGreyness, gGreyness, 0xFF);
				gBackgroundTexture.render(0, 0);
				//SDL_RenderClear(gRenderer);

				//Render buttons
				//for (int i = 0; i < TOTAL_BUTTONS; ++i)
				//{
				//	gButtons[i].render(&gButtonSpriteSheetTexture, gSpriteClips);
				//}

				//Render characters

				int x = (SCREEN_WIDTH - gCharacterTexture.getWidth()) / 2;
				int y = (SCREEN_HEIGHT / 2 - gCharacterTexture.getHeight() / 2);

				int xShadow = x + xShadowOffset;
				int yShadow = y + yShadowOffset;
				int widthShadow = gShadowTexture.getWidth() - widthCut;
				int heightShadow = gShadowTexture.getHeight() - heightCut;
				widthShadow = std::max(widthShadow, 0);
				heightShadow = std::max(heightShadow, 0);

				SDL_Rect clip = { 0,0, widthShadow, heightShadow };
				SDL_Point center = { gShadowTexture.getWidth()/2 - xRotate, gShadowTexture.getHeight() / 2 - yRotate };
				gShadowTexture.render(xShadow, yShadow, &clip, angle=angle +0.2, &center);

				gCharacterTexture.render(x, y);


				//gBackgroundTexture.render(0, 0);

				//Update screen
				SDL_RenderPresent(gRenderer);
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}