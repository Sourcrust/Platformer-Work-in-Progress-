#include <SDL.h>
#include "Person.h"

extern const int SCALE;

Person::Person(int xi, int yi, int wi, int hi) : Box(xi, yi, wi, hi){
    update();
}

void Person::draw(SDL_Renderer* &r){
    update();
    if(dashState==0){
        SDL_SetRenderDrawColor(r,0xBC,0x0B,0x0B,0xFF);
    }
    else if(dashState==1){
        SDL_SetRenderDrawColor(r,0xFF,0xFF,0xFF,0xFF);
    }
    else if(dashState==2){
        SDL_SetRenderDrawColor(r,24,174,249,0xFF);
    }
    SDL_RenderFillRect(r,&drawInfo);
}

void Person::update(){                  // prepares platforms for drawing
    drawInfo.x=x/SCALE;                 // dont know why i update all the time, but incase the platforms will move sometime
    drawInfo.y=y/SCALE;
    drawInfo.w=w/SCALE;
    drawInfo.h=h/SCALE;
}
