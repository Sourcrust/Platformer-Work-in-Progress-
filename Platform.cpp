#include <SDL.h>
#include "Platform.h"
                    // rectangle for drawing
Platform::Platform(int xi, int yi, int wi, int hi) : Box(xi, yi, wi, hi){
    update();
}

void Platform::update(){                          // prepares platforms for drawing
    drawInfo.x=x/SCALE;                 // dont know why i update all the time, but incase the platforms will move sometime
    drawInfo.y=y/SCALE;
    drawInfo.w=w/SCALE;
    drawInfo.h=h/SCALE;
}

