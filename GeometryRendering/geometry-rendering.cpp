/*This source code copyrighted by Lazy Foo' Productions (2004-2019)
and may not be redistributed without written permission.*/

//Using SDL and standard IO
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;


bool init(); // Initializes the gWindow and the screenSurface
bool loadMedia(); // Load the tiger image onto the gTiger surface
void close(); // Frees up memory and closes the gWindow

SDL_Window* gWindow = NULL; // The window we are reffering to
SDL_Renderer* gRenderer = NULL; //The window renderer
SDL_Texture* gTexture = NULL; //Current displayed texture

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

    while(!quit)
    {
        while(SDL_PollEvent(&e) != 0)
        {

            switch(e.type)
            {
            case SDL_QUIT:
                quit = true;
                break;
            }


            SDL_SetRenderDrawColor( gRenderer, 0xFF,0xFF,0xFF,0xFF);
            SDL_RenderClear( gRenderer );

            SDL_Rect fillRect = {SCREEN_WIDTH /4, SCREEN_HEIGHT/4, SCREEN_WIDTH/2, SCREEN_HEIGHT/2};
            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
            SDL_RenderFillRect(gRenderer, &fillRect);

            SDL_Rect outlineRect= {SCREEN_WIDTH /6, SCREEN_HEIGHT/6, SCREEN_WIDTH*2/3, SCREEN_HEIGHT*2/3};
            SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0x00, 0x00);
            SDL_RenderFillRect(gRenderer, &outlineRect);

            SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0xFF, 0xFF);
            SDL_RenderDrawLine(gRenderer,0, SCREEN_HEIGHT/2, SCREEN_WIDTH , SCREEN_HEIGHT/2);

            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF,0x00, 0xFF);
            for( int i = 0; i < SCREEN_HEIGHT; i+=4) {
                SDL_RenderDrawPoint(gRenderer,SCREEN_WIDTH/2, i);
            }

            SDL_RenderPresent(gRenderer);
        }
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

    gWindow = SDL_CreateWindow( "Window name",
                                500, // X Position
                                500, // Y Position
                                SCREEN_WIDTH,
                                SCREEN_HEIGHT,
                                SDL_WINDOW_SHOWN
                              );

    if(gWindow == NULL)
    {
        printf("An error was encountered:\n SDL_ERROR: %s", SDL_GetError() );
        return false;
    }

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
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

    // Destroy and nullifies the texture
    SDL_DestroyTexture(gTexture);
    gTexture = NULL;

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
    return success;
}

