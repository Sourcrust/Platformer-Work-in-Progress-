#ifndef PERSON_H
#define PERSON_H

#include <SDL.h>
#include "Box.h"

class Person: public Box{
    public:
        int DASH_MAG=120;                     // Dash Magnitude
        int DASH_DURATION=20;                 // how many frames is the dash
        int SLIDE_SPEED=30;
        int WALK_SPEED=65;
        int JUMP_POWER=110;
        int CLIMB_SPEED=20;
        int JUMP_DURATION=30;

        int jumpTimer=0;
        int dashState=0;                            // 0 is can dash, 1 is dashing, 2 is cant dash
        bool canJump=true;
        int dashTimer;
        bool facingRight=true;                      // checks if facing right
        int againstWall=-1;
        bool wallRight=false;
        int xSpeed=0,ySpeed=0;
        bool jumpReleased=true;

        SDL_Rect drawInfo;                      // rectangle for drawing
        Person(int xi,int yi, int wi, int hi);
        void draw(SDL_Renderer* &r);
        void update();
};

#endif // PERSON_H
