/*This source code copyrighted by Lazy Foo' Productions (2004-2019)
and may not be redistributed without written permission.*/

//Using SDL and standard IO
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include "lTexture.h"

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;


bool init(); // Initializes the gWindow and the screenSurface
bool loadMedia(); // Load the tiger image onto the gTiger surface
void close(); // Frees up memory and closes the gWindow

// The window we are reffering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

LTexture gArrowTexture;

int main( int argc, char* args[] )
{

    if(!init())
    {
        printf("Failed to initialize");
        return 0;
    }

    if(!loadMedia())
    {
        printf("Failed to load media");
        return 0;
    }

    bool quit = false;
    SDL_Event e;

    double degree = 0.0;
    SDL_RendererFlip flipType = SDL_FLIP_NONE;

    while(!quit)
    {
        while(SDL_PollEvent(&e) != 0)
        {

            switch(e.type)
            {
                case SDL_QUIT:
                    quit = true;
                break;
                case SDL_KEYDOWN:
                    switch(e.key.keysym.sym)
                    {
                    case SDLK_a:
                            degree -= 60;
                        break;
                        case SDLK_d:
                            degree += 60;
                        break;
                        case SDLK_q:
                            flipType = SDL_FLIP_HORIZONTAL;
                        break;
                        case SDLK_w:
                            flipType = SDL_FLIP_NONE;
                        break;
                        case SDLK_e:
                            flipType = SDL_FLIP_VERTICAL;
                        break;
                    }

                break;
            }
        }

        SDL_SetRenderDrawColor(gRenderer, 255,255,255,255);
        SDL_RenderClear(gRenderer);

        gArrowTexture.render((SCREEN_WIDTH - gArrowTexture.getWidth()) / 2,
                             (SCREEN_HEIGHT - gArrowTexture.getHeight()) / 2,
                             NULL,
                             degree,
                             NULL,
                             flipType
                             );

        SDL_RenderPresent(gRenderer);
    }

    return 0;
}

bool init()
{
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        return false;
    }

    if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
        printf("Warning: Linear texture filtering not enabled");
    }

    gWindow = SDL_CreateWindow( "Window name",
                                100, // X Position
                                100, // Y Position
                                SCREEN_WIDTH,
                                SCREEN_HEIGHT,
                                SDL_WINDOW_SHOWN
                              );

    if(gWindow == NULL)
    {
        printf("An error was encountered:\n SDL_ERROR: %s", SDL_GetError() );
        return false;
    }

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(gRenderer == NULL)
    {
        printf("Renderer couldn't be created! SDL Error: %s", SDL_GetError());
        return false;
    }

    SDL_SetRenderDrawColor( gRenderer, 0xFF,0xFF,0xFF,0xFF);

    //Initialize PNG loading
    int IMGFlags = IMG_INIT_PNG;
    if(!(IMG_Init(IMGFlags) & IMGFlags))
    {
        printf("SDL Image couldn't be initialized!SDL IMG ERROR:%s", IMG_GetError());
        return false;
    }

    return true;
}

void close()
{

    gArrowTexture.free();

    // Destroys and nullifies the window
    SDL_DestroyRenderer(gRenderer);
    gRenderer = NULL;

    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    //Quits the SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

bool loadMedia()
{
    bool success = true;


    gArrowTexture.setRenderer(gRenderer);
    if(!gArrowTexture.loadFromFile("resources/arrow.png")) {
        printf("Failed to load arrow");
        success = false;
    }

    return success;
}
