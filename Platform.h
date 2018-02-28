#ifndef PLATFORM_H
#define PLATFORM_H

#include <SDL.h>
#include "Box.h"

extern const int SCALE;

class Platform: public Box{
    public:
        SDL_Rect drawInfo;                      // rectangle for drawing
        Platform(int xi,int yi, int wi, int hi);
        void update();
};

#endif

