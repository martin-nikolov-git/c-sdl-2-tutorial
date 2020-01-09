#ifndef LTEXTURE_H_INCLUDED
#define LTEXTURE_H_INCLUDED

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>

class LTexture {
    public:
        //Initializes variable
        LTexture();

        //Deallocates memory
        ~LTexture();

        //Loads image at specified path
        bool loadFromFile(std::string path);

        //Create images from font string
        bool loadFromRenderedText( std::string textureText, SDL_Color textColor);

        //Deallocates texture;
        void free();

        //Renders texture at given point
        void render(int x,
                    int y,
                    SDL_Rect* clip = NULL,
                    double angle = 0.0,
                    SDL_Point* center = NULL,
                    SDL_RendererFlip flip = SDL_FLIP_NONE);

        //Set blending mode
        void setBlendMode(SDL_BlendMode blending);

        //Set alpha modulation
        void setAlpha(Uint8 alpha);

        //Image dimensions
        int getHeight();
        int getWidth();

        //Sets the renderer
        void setRenderer(SDL_Renderer* renderer);
        void setFont(TTF_Font* font);

    private:
        SDL_Texture* mTexture;
        SDL_Renderer* mRenderer;
        TTF_Font* mFont;


        int mHeight;
        int mWidth;
};

#endif // LTEXTURE_H_INCLUDED
