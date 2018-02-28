#include <SDL.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include <vector>
#include <iostream>

#include "Includes.h"

point r;

const int HEIGHT=12000;                     // height of the map, one unit of height does not equal one pixel
const int WIDTH=15000;
const int SCALE=20;                         // how many units of height/width is a pixel
const int SCREEN_WIDTH=WIDTH/SCALE;         // screen width and height
const int SCREEN_HEIGHT=HEIGHT/SCALE;
const int GRAVITY=3;                        // force of gravity, how much delta y changes per frame

std::vector<Platform> platforms;            // vector storing all the platforms
int numPlatforms;
const std::string MAP_FILE="MapFile.txt";   // File for all platforms

SDL_Window* window=NULL;                    // dont touch these
SDL_Surface* screenSurface=NULL;
SDL_Renderer* renderer=NULL;

SDL_Event e;
bool keysPressed[323]={false};              // array storing which buttons are pressed
bool shouldQuit=false;                      // also dont touch this

// VARIABLES FOR PLAYER

Person p={WIDTH/2,HEIGHT/2,400,800};

void handleEvents(){        // event handler (it just updates the boolean array for keybindings)
    // all it does is update the boolean array
    while(SDL_PollEvent(&e)!=0){
        if(e.type==SDL_KEYDOWN&&e.key.keysym.sym<323){
            keysPressed[e.key.keysym.sym]=true;
        }
        else if(e.type==SDL_KEYUP&&e.key.keysym.sym<323){
            keysPressed[e.key.keysym.sym]=false;
        }
        else if(e.type==SDL_QUIT){
            shouldQuit=true;
        }
    }
}

void eraseScreen(){         // cls
    SDL_SetRenderDrawColor(renderer,0x00,0x00,0x00,0x00);
    SDL_RenderClear(renderer);
}


//is overlapped condition -> for each box
//temporary new p.y & p.x before actually doing the change
//if it is overlapped, cancel jump and reengage gravity and keep prev p.y and p.x and cancel dash
//otherwise update actual to the temp

bool checkPoint(int pX,int pY,int x1,int y1,int x2, int y2){
    return pX>x1&&pX<x2&&pY>y1&&pY<y2;
}

bool isInside(Person p, Box boxObject){
    if(checkPoint(p.x,p.y,boxObject.x,boxObject.y,boxObject.x+boxObject.w,boxObject.y+boxObject.h)
    || checkPoint(p.x+p.w,p.y,boxObject.x,boxObject.y,boxObject.x+boxObject.w,boxObject.y+boxObject.h)
    || checkPoint(p.x,p.y+p.h,boxObject.x,boxObject.y,boxObject.x+boxObject.w,boxObject.y+boxObject.h)
    || checkPoint(p.x+p.w,p.y+p.h,boxObject.x,boxObject.y,boxObject.x+boxObject.w,boxObject.y+boxObject.h)){
        return true;
    }
    else if(checkPoint(boxObject.x,boxObject.y,p.x,p.y,p.x+p.w,p.y+p.h)
    || checkPoint(boxObject.x+boxObject.w,boxObject.y,p.x,p.y,p.x+p.w,p.y+p.h)
    || checkPoint(boxObject.x+boxObject.w,boxObject.y+boxObject.h,p.x,p.y,p.x+p.w,p.y+p.h)
    || checkPoint(boxObject.x,boxObject.y+boxObject.h,p.x,p.y,p.x+p.w,p.y+p.h)){
        return true;
    }
    return false;
}

bool touches(int leftP,int rightP,int pX1, int pX2){
    return (rightP>=pX1&&rightP<=pX2)||(leftP>=pX1&&leftP<=pX2)||(leftP<=pX1&&rightP>=pX2)||(leftP>=pX1&&rightP<=pX2);
}

bool standardCheck(int i){
    if(p.ySpeed>0){
        p.canJump=true;
        if(p.xSpeed==0){
            p.y=platforms[i].y-p.h;
            p.ySpeed=0;
            if(!keysPressed[DASH_KEY]){                             // replenishes dash
                p.dashState=0;
            }
        }
        else{
            double m=(double)p.ySpeed/(double)p.xSpeed;
            double b=((p.y+p.h)-m*p.x);
            //y=mx+b, x=(y-b)/m
            double newX1=(platforms[i].y-b)/m;
            double newX2=newX1+p.w;
            if(touches(platforms[i].x,platforms[i].x+platforms[i].w,newX1,newX2)){
                if(p.y-p.ySpeed!=platforms[i].y-p.h-1){
                    p.x=(int)newX1;
                }
                p.y=platforms[i].y-p.h-1;
                p.ySpeed=0;
                if(!keysPressed[DASH_KEY]){                         // replenishes dash
                    p.dashState=0;
                }
            }
            else if(platforms[i].x>=newX2){
                p.x=platforms[i].x-p.w-1;
                p.y=m*(platforms[i].x-p.w)+b-p.h;
                p.xSpeed=0;
            }
            else if(platforms[i].x+platforms[i].w<=newX1){
                p.x=platforms[i].x+platforms[i].w+1;
                p.xSpeed=0;
                p.y=m*(platforms[i].x+platforms[i].w)+b-p.h;
            }
        }
    }
    else if(p.ySpeed<0){
        if(p.xSpeed==0){
            p.y=platforms[i].y+platforms[i].h;
            p.ySpeed=0;
        }
        else{
            double m=(double)p.ySpeed/(double)p.xSpeed;
            double b=(p.y-m*p.x);
            //y=mx+b, x=(y-b)/m
            double newX1=(platforms[i].y+platforms[i].h-b)/m;
            double newX2=newX1+p.w;
            if(touches(platforms[i].x,platforms[i].x+platforms[i].w,newX1,newX2)){
                p.y=platforms[i].y+platforms[i].h+1;
                p.ySpeed=0;
                p.x=(int)newX1;
            }
            else if(platforms[i].x>=newX2){
                p.x=platforms[i].x-p.w-1;
                p.y=m*(platforms[i].x-p.w)+b;
                p.xSpeed=0;
            }
            else if(platforms[i].x+platforms[i].w<=newX1){
                p.x=platforms[i].x+platforms[i].w+1;
                p.xSpeed=0;
                p.y=m*(platforms[i].x+platforms[i].w)+b;
            }
        }
    }
    else{
        if(p.xSpeed>0){
            p.x=platforms[i].x-p.w-1;
            p.xSpeed=0;
        }
        else{
                printf("&&&&&&&&");
            p.x=platforms[i].x+platforms[i].w+1;
            p.xSpeed=0;
        }
    }
}

void checkCollided(){
    for(int i=0; i<numPlatforms; i++){
        //for each corner of the player -> later make this better by checking edges as well
        //this might not work for platforms that are smaller in width or height than the player
        if(isInside(p,platforms[i])){
            standardCheck(i);                   // checks everything for normal collosion (not dash)
        }
    }
}

void climbOverIfPossible(){
    bool possible=true;
    for(int i=0;i<numPlatforms;i++){
        if(i!=p.againstWall){
            if(touches(p.x-1,p.x+p.w+1,platforms[i].x,platforms[i].x+platforms[i].w)&&touches(p.y,p.y+p.h,platforms[i].y,platforms[i].y+platforms[i].h)){
                possible=false;
                break;
            }
        }
    }
    if(possible){

        p.y=platforms[p.againstWall].y-p.h-10;
        if(p.wallRight){
            p.x=platforms[p.againstWall].x-p.w+10;
        }
        else{
            p.x=platforms[p.againstWall].x+platforms[p.againstWall].w-10;
        }
        p.xSpeed=0;
        p.ySpeed=0;
        p.againstWall=-1;
    }
}

void checkClimbedTooFar(){
    if(p.ySpeed>0){
        printf("4");
        if(p.y>platforms[p.againstWall].y+platforms[p.againstWall].h){
            p.againstWall=-1;
            p.ySpeed=0;
        }
    }
    else if(p.ySpeed<0){
        if((p.y+p.h<platforms[p.againstWall].y)){
            climbOverIfPossible(); // WORK ON THISSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS
        }
    }
}

void moveRect(){                        // Moves the player
    if(p.dashState!=1&&p.againstWall==-1){         // gravity only affects when theres no dash
        p.ySpeed+=GRAVITY;
    }
    p.y+=p.ySpeed;
    p.x+=p.xSpeed;
    checkCollided();
    if(p.againstWall!=-1){
        checkClimbedTooFar();
    }

    /*
    if(p.x<0){                                // these two ifs must be changed for platform conditions sometime
        p.x=0;
    }
    else if(p.x/SCALE>WIDTH-p.w){
        p.x=WIDTH-p.w;
    }*/
}

void drawBoxes(){                           // draws the platforms
    SDL_SetRenderDrawColor(renderer,0xFF,0xFF,0xFF,0xFF);
    for(int i=0;i<numPlatforms;i++){
        platforms[i].update();
        SDL_RenderFillRect(renderer,&(platforms[i].drawInfo));
    }
}

void draw(){
    drawBoxes();
    p.draw(renderer);
    SDL_RenderPresent(renderer);
};

int main(int argc, char* args[]){
    if(!init(window,renderer,screenSurface,SCREEN_WIDTH,SCREEN_HEIGHT)){                // initializes SDL
        return 1;
    }

    if(!loadMap(numPlatforms,platforms,MAP_FILE)){             // reads from textfile
        return 1;
    }

    // Main loop
    while(!shouldQuit){
        handleEvents();         // KeyListener
        eraseScreen();          // cls
        checkInputs(p,keysPressed,platforms);          // reads inputs from boolean array
        moveRect();             // moves the player
        draw();                 // draws player, then platforms
        if(p.dashTimer>0){        // timer for the dash. Dahs runs out once dahs timer goes down to 0
            p.dashTimer--;
            if(p.dashTimer==0){
                p.dashState=2;    // dashState=2 is when you cant dash again
                p.xSpeed/=4;
                p.ySpeed/=4;
            }
        }
        SDL_Delay(10);          // delay
        printf("%i ",p.againstWall);
    }
    quit(window);
    return 0;

}
