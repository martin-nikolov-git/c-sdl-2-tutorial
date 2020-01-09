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

class LTexture {
    public:
        //Initializes variable
        LTexture();

        //Deallocates memory
        ~LTexture();

        //Loads image at specified path
        bool loadFromFile(std::string path);

        //Deallocates texture;
        void free();

        //Renders texture at given point
        void render(int x,int y);

        //Image dimensions
        int getHeight();
        int getWidth();

    private:
        SDL_Texture* mTexture;

        int mHeight;
        int mWidth;
};

bool init(); // Initializes the gWindow and the screenSurface
bool loadMedia(); // Load the tiger image onto the gTiger surface
void close(); // Frees up memory and closes the gWindow

// The window we are reffering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

LTexture gFooTexture;
LTexture gBackgroundTexture;


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


            SDL_SetRenderDrawColor(gRenderer, 255,255,255,255);
            SDL_RenderClear(gRenderer);

            gBackgroundTexture.render(0,0);
            gFooTexture.render(240,190);

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

    gFooTexture.free();
    gBackgroundTexture.free();

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

    if(!gFooTexture.loadFromFile("resources/foo.png")) {
        printf("Failed to load foo");
        success = false;
    }

    if(!gBackgroundTexture.loadFromFile("resources/background.png")) {
        printf("Failed to load background");
        success = false;
    }

    return success;
}


LTexture::LTexture()
{
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}

LTexture::~LTexture()
{
    free();
}

bool LTexture::loadFromFile(std::string path)
{
    //Deallocates texture
    free();

    SDL_Texture* newTexture = NULL;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if(loadedSurface == NULL) {
        printf("Unable to load image at %s!Error: %s", path.c_str(), IMG_GetError());
    } else {

        //Color key image
        SDL_SetColorKey(
            loadedSurface,
            SDL_TRUE,
            SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF)
        );

        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if( newTexture == NULL) {
            printf("Unable to create texture from %s. Error: %s", path.c_str(), SDL_GetError());
        } else {
            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h;
        }

        //Get rid of old texture
        SDL_FreeSurface(loadedSurface);
    }

    mTexture = newTexture;
    return mTexture != NULL;
}

void LTexture::free()
{
    if( mTexture != NULL) {
        SDL_DestroyTexture(mTexture);
        mTexture = NULL;
        mWidth = NULL;
        mHeight = NULL;
    }
}

void LTexture::render(int x, int y)
{
    SDL_Rect renderQuad = {x, y, mWidth, mHeight};
    SDL_RenderCopy(gRenderer, mTexture, NULL, &renderQuad);
}

int LTexture::getHeight()
{
    return mHeight;
}

int LTexture::getWidth()
{
    return mWidth;
}
