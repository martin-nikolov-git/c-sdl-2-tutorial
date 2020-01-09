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
        void render(int x,int y, SDL_Rect* clip = NULL);

        //Set blending mode
        void setBlendMode(SDL_BlendMode blending);

        //Set alpha modulation
        void setAlpha(Uint8 alpha);

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


const int WALKING_ANIMATION_FRAMES = 4;
SDL_Rect gSpriteClips[WALKING_ANIMATION_FRAMES];
LTexture gSpriteSheetTexture;


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
    int frame = 0;

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
        }

        SDL_SetRenderDrawColor(gRenderer, 255,255,255,255);
        SDL_RenderClear(gRenderer);

        SDL_Rect* currentClip = &gSpriteClips[frame/4];
        gSpriteSheetTexture.render(
                                   (SCREEN_WIDTH - currentClip->w) / 2,
                                   (SCREEN_HEIGHT - currentClip->h) / 2,
                                   currentClip
                                   );

        SDL_RenderPresent(gRenderer);

        ++frame;
        if(frame/4 >= WALKING_ANIMATION_FRAMES)
        {
            frame = 0;
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

    gSpriteSheetTexture.free();

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


    if(!gSpriteSheetTexture.loadFromFile("resources/foo.png")) {
        printf("Failed to load fadeout");
        success = false;
    } else {
        gSpriteClips[0].x = 0;
        gSpriteClips[0].y = 0;
        gSpriteClips[0].w = 64;
        gSpriteClips[0].h = 205;

        gSpriteClips[1].x = 64;
        gSpriteClips[1].y = 0;
        gSpriteClips[1].w = 64;
        gSpriteClips[1].h = 205;

        gSpriteClips[2].x = 128;
        gSpriteClips[2].y = 0;
        gSpriteClips[2].w = 64;
        gSpriteClips[2].h = 205;

        gSpriteClips[3].x = 196;
        gSpriteClips[3].y = 0;
        gSpriteClips[3].w = 64;
        gSpriteClips[3].h = 205;
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

void LTexture::render(int x, int y, SDL_Rect* clip)
{
    SDL_Rect renderQuad = {x, y, mWidth, mHeight};

    if(clip!=NULL) {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    SDL_RenderCopy(gRenderer, mTexture, clip, &renderQuad);
}

int LTexture::getHeight()
{
    return mHeight;
}

int LTexture::getWidth()
{
    return mWidth;
}

void LTexture::setBlendMode(SDL_BlendMode blending)
{
    SDL_SetTextureBlendMode(mTexture, blending);
}

void LTexture::setAlpha(Uint8 alpha)
{
    SDL_SetTextureAlphaMod(mTexture, alpha);
}
