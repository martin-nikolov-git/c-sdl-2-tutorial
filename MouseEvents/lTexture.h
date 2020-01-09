#ifndef LTEXTURE_H_INCLUDED
#define LTEXTURE_H_INCLUDED

#include <SDL.h>
#include <SDL_image.h>
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

        #if defined(_SDL_TTF_H) || defined(SDL_TTF_H)
        //Create images from font string
        bool loadFromRenderedText( std::string textureText, SDL_Color textColor);
        #endif

        //Deallocates texture;
        void free();

        //Renders texture at given point
        void render(int x,
                    int y,
                    SDL_Rect* clip = NULL,
                    double angle = 0.0,
                    SDL_Point* center = NULL,
                    SDL_RendererFlip flip = SDL_FLIP_NONE);


        //Image dimensions
        int getHeight();
        int getWidth();

        //Sets the renderer
        void setRenderer(SDL_Renderer* renderer);

        #if defined(_SDL_TTF_H) || defined(SDL_TTF_H)
        void setFont(TTF_Font* font);
        #endif

        //Set blending mode
        void setBlendMode(SDL_BlendMode blending);

        //Set alpha modulation
        void setAlpha(Uint8 alpha);

    private:
        SDL_Texture* mTexture;
        SDL_Renderer* mRenderer;

        #if defined(_SDL_TTF_H) || defined(SDL_TTF_H)
        TTF_Font* mFont;
        #endif


        int mHeight;
        int mWidth;
};

#endif // LTEXTURE_H_INCLUDED
