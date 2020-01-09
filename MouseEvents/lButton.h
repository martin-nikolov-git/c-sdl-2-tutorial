#ifndef LBUTTON_H_INCLUDED
#define LBUTTON_H_INCLUDED


enum LButtonSprite
{
    BUTTON_SPRITE_MOUSE_OUT = 0,
    BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
    BUTTON_SPRITE_MOUSE_DOWN = 2,
    BUTTON_SPRITE_MOUSE_UP = 3,
    BUTTON_SPRITE_TOTAL = 4
};


#include <SDL.h>
#include "lTexture.h"

const int BUTTON_WIDTH = 300;
const int BUTTON_HEIGHT = 200;
const int TOTAL_BUTTONS = 4;

class LButton
{
    public:
        static SDL_Rect gSpriteClips[BUTTON_SPRITE_TOTAL];

        //Constructor
        LButton();

        //Sets the top left position
        void setPosition(int x, int y);

        //Handle mouse events
        void handleEvent(SDL_Event* event);

        void setTexture(LTexture* texture);


        //Show button sprite
        void render();

    private:
        SDL_Point mPosition;
        LButtonSprite mCurrentSprite;
        LTexture* mButtonTexture;

};

#endif
