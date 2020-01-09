/*This source code copyrighted by Lazy Foo' Productions (2004-2019)
and may not be redistributed without written permission.*/

//Using SDL and standard IO
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include "lTexture.h"
#include "lButton.h"

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

bool init(); // Initializes the gWindow and the screenSurface
void close(); // Frees up memory and closes the gWindow
bool loadMedia(); // Load the tiger image onto the gTiger surface

SDL_Window* gWindow = NULL; // The window we are reffering to

SDL_Renderer* gRenderer = NULL; //The window renderer

LTexture gButtonSpriteTexture;
LButton gButtons[TOTAL_BUTTONS];


int main( int argc, char* args[] )
{

    if(!init()) {
        printf("Failed to initialize");
        return 0;
    }

    if(!loadMedia()) {
        printf("Failed to load media");
        return 0;
    }

    bool quit = false;
    SDL_Event e;

    while(!quit) {
        while(SDL_PollEvent(&e) != 0) {
            switch(e.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
            }

            for(int i = 0; i < TOTAL_BUTTONS; ++i) {
                gButtons[i].handleEvent(&e);
            }
        }

        //Clear screen
        SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderClear( gRenderer );

        //Render buttons
        for( int i = 0; i < TOTAL_BUTTONS; ++i )
        {
            gButtons[ i ].render();
        }

        //Update screen
        SDL_RenderPresent( gRenderer );
    }

	return 0;
}

bool init() {
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

	if(gWindow == NULL) {
        printf("An error was encountered:\n SDL_ERROR: %s", SDL_GetError() );
        return false;
	}

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    if(gRenderer == NULL) {
        printf("Renderer couldn't be created! SDL Error: %s", SDL_GetError());
        return false;
    }

    SDL_SetRenderDrawColor( gRenderer, 0xFF,0xFF,0xFF,0xFF);

    //Initialize PNG loading
    int IMGFlags = IMG_INIT_PNG;
    if(!(IMG_Init(IMGFlags) & IMGFlags)) {
        printf("SDL Image couldn't be initialized!SDL IMG ERROR:%s", IMG_GetError());
        return false;
    }

    return true;
}

void close() {

    // Destroy and nullifies the texture
    gButtonSpriteTexture.free();

    // Destroys and nullifies the window
    SDL_DestroyRenderer(gRenderer);
    gRenderer = NULL;

    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    //Quits the SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

bool loadMedia() {
    bool success = true;

    gButtonSpriteTexture.setRenderer(gRenderer);
    if(!gButtonSpriteTexture.loadFromFile("resources/button.png")) {
        printf("Failed to load button texture!\n%s", SDL_GetError());
        success = false;
    } else {
        for(int i = 0; i< BUTTON_SPRITE_TOTAL; ++i) {
            LButton::gSpriteClips[i].x = 0;
            LButton::gSpriteClips[i].y = i * 200;
            LButton::gSpriteClips[i].w = BUTTON_WIDTH;
            LButton::gSpriteClips[i].h = BUTTON_HEIGHT;
        }

        gButtons[0].setTexture(&gButtonSpriteTexture);
        gButtons[0].setPosition(0,0);

        gButtons[1].setTexture(&gButtonSpriteTexture);
        gButtons[1].setPosition(SCREEN_WIDTH-BUTTON_WIDTH, 0);

        gButtons[2].setTexture(&gButtonSpriteTexture);
        gButtons[2].setPosition(0, SCREEN_HEIGHT - BUTTON_HEIGHT);

        gButtons[3].setTexture(&gButtonSpriteTexture);
        gButtons[3].setPosition(SCREEN_WIDTH-BUTTON_WIDTH, SCREEN_HEIGHT - BUTTON_HEIGHT);
    }


    return success;
}

