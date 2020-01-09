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

enum KeyPressSurfaces {
    KEY_PRESS_SURFACE_DEFAULT,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_TOTAL
};

bool init(); // Initializes the gWindow and the screenSurface
bool loadMedia(); // Load the tiger image onto the gTiger surface
void close(); // Frees up memory and closes the gWindow
SDL_Surface* load_bmp_surface(std::string path);
SDL_Surface* load_png_surface(std::string path);

SDL_Window* gWindow = NULL; // The window we are reffering to
SDL_Surface* screenSurface = NULL; // The surface contained in the window
SDL_Surface* gKeyPressSurfaces[KEY_PRESS_SURFACE_TOTAL]; // An array of available surfaces
SDL_Surface* gCurrentSurface = NULL; // THE CURRENT SURFACE
SDL_Surface* gPngSurface = NULL;

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

    gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];

    while(!quit) {
        while(SDL_PollEvent(&e) != 0) {

            switch(e.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
            }
            //Apply the image
            SDL_Rect stretchRect;
            stretchRect.x = 0;
            stretchRect.y = 0;
            stretchRect.w = SCREEN_WIDTH;
            stretchRect.h = SCREEN_HEIGHT;

            // SDL_BlitScaled(gCurrentSurface, NULL, screenSurface, &stretchRect);
            SDL_BlitSurface( gPngSurface, NULL, screenSurface, NULL );

            //Update the surface
            SDL_UpdateWindowSurface( gWindow );
        }
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

    int imgFlags = IMG_INIT_PNG;
    if( !(IMG_Init(imgFlags) & imgFlags)) {
        printf("SDL_Image could not initialize! SDL_Image Error:%s\n", IMG_GetError());
        return false;
    }

	screenSurface = SDL_GetWindowSurface(gWindow);

    return true;
}

void close() {
    //Frees and nullifies the loaded surface
    SDL_FreeSurface(gCurrentSurface);

    // Destroys and nullifies the gWindow
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    //Quits the SDL subsystems
    SDL_Quit();
}

bool loadMedia() {
    bool success = true;

    gPngSurface = load_png_surface("resources/loaded.png");
    if(gPngSurface == NULL)
    {
        printf("Failed to load PNG!\n");
        success = false;
    }

    return success;
    // DEFAULT
    gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] = load_bmp_surface("resources/press.bmp");
    if( gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] == NULL) {
        printf("Failed to load default image!\n");
        success = false;
    }

    //UP
    gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] = load_bmp_surface("resources/up.bmp");
    if( gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] == NULL) {
        printf("Failed to load up image!\n");
        success = false;
    }

    gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] = load_bmp_surface("resources/down.bmp");
    if( gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] == NULL) {
        printf("Failed to load down image!\n");
        success = false;
    }

    gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] = load_bmp_surface("resources/left.bmp");
    if( gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] == NULL) {
        printf("Failed to load left image!\n");
        success = false;
    }

    gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] = load_bmp_surface("resources/right.bmp");
    if( gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] == NULL) {
        printf("Failed to load right image!\n");
        success = false;
    }

    return success;
}

SDL_Surface* load_bmp_surface(std::string path) {

    SDL_Surface* loadedSurface = SDL_LoadBMP(path.c_str());
    SDL_Surface* optimizedSurface = NULL;

    if( loadedSurface == NULL) {
        printf("Unable to load image at path %s. Error: %s", path.c_str(), SDL_GetError());
    } else {
        optimizedSurface = SDL_ConvertSurface(loadedSurface, screenSurface->format, 0);
        if(optimizedSurface == NULL) {
            printf("Unable to optimize image %s! SDL Error: %s", path.c_str(), SDL_GetError());
            return loadedSurface;
        }

        SDL_FreeSurface(loadedSurface);
    }

    return optimizedSurface;
}

SDL_Surface* load_png_surface(std::string path) {

    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    SDL_Surface* optimizedSurface = NULL;

    if( loadedSurface == NULL) {
        printf("Unable to load image at path %s. Error: %s", path.c_str(), IMG_GetError());
    } else {
        optimizedSurface = SDL_ConvertSurface(loadedSurface, screenSurface->format, 0);
        if(optimizedSurface == NULL) {
            printf("Unable to optimize image %s! SDL Error: %s", path.c_str(), IMG_GetError());
            return loadedSurface;
        }

        SDL_FreeSurface(loadedSurface);
    }

    return optimizedSurface;
}
