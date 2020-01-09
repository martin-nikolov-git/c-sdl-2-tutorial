
#include "lTexture.h"

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

        newTexture = SDL_CreateTextureFromSurface(mRenderer, loadedSurface);
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
        mRenderer = NULL;
        mTexture = NULL;
        mWidth = NULL;
        mHeight = NULL;
    }
}

void LTexture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
    SDL_Rect renderQuad = {x, y, mWidth, mHeight};

    if(clip!=NULL) {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    SDL_RenderCopyEx(mRenderer, mTexture, clip, &renderQuad, angle, center, flip);
}

int LTexture::getHeight()
{
    return mHeight;
}

int LTexture::getWidth()
{
    return mWidth;
}

void LTexture::setRenderer(SDL_Renderer* gRenderer)
{
    mRenderer = gRenderer;
}

void LTexture::setBlendMode(SDL_BlendMode blending)
{
    SDL_SetTextureBlendMode(mTexture, blending);
}

void LTexture::setAlpha(Uint8 alpha)
{
    SDL_SetTextureAlphaMod(mTexture, alpha);
}
