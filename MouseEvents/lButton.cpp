#include "lButton.h"


SDL_Rect LButton::gSpriteClips[BUTTON_SPRITE_TOTAL] = {
    { 300, 400, 50, 50 },
    { 300, 400, 50, 50 },
    { 300, 400, 50, 50 },
    { 300, 400, 50, 50 }
};

LButton::LButton() {
    mPosition.x = 0;
    mPosition.y = 0;

    mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
}

void LButton::setPosition(int x, int y) {
    mPosition.x = x;
    mPosition.y = y;
}

void LButton::handleEvent(SDL_Event* event) {
    //If mouse event happens
    if( event->type != SDL_MOUSEMOTION &&
        event->type != SDL_MOUSEBUTTONDOWN &&
        event->type != SDL_MOUSEBUTTONUP) {
        return;
    }

    int x,y;
    //Get position
    SDL_GetMouseState(&x,&y);

    bool inside = true;

    //Check if mouse is in bounds
    if( x < mPosition.x ||
        x > mPosition.x + BUTTON_WIDTH ||
        y < mPosition.y ||
        y > mPosition.y + BUTTON_HEIGHT) {
        inside = false;
    }

    if(!inside) {
        mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
    } else {
        //Check mouse event

        switch(event->type) {
            case SDL_MOUSEMOTION:
                mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
            break;

            case SDL_MOUSEBUTTONDOWN:
                mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
            break;

            case SDL_MOUSEBUTTONUP:
                mCurrentSprite = BUTTON_SPRITE_MOUSE_UP;
            break;
        }
    }
}

void LButton::setTexture(LTexture* texture) {
    mButtonTexture = texture;
}

void LButton::render() {
    mButtonTexture->render(mPosition.x, mPosition.y, &gSpriteClips[mCurrentSprite]);
}

