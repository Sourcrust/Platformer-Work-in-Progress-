#include <SDL.h>
#include "Platform.h"
#include "Person.h"
#include "CheckInputs.h"
#include <vector>
#include <stdio.h>

extern const int DASH_KEY,DASH_MAG,UP_KEY,LEFT_KEY,DOWN_KEY,RIGHT_KEY,HOLD_KEY,JUMP_KEY;

void dashCase(Person &p,bool keysPressed[343]){
    p.dashState=1;
    p.dashTimer=p.DASH_DURATION;
    p.againstWall=-1;
    if(keysPressed[UP_KEY]){
        if(keysPressed[LEFT_KEY]){
            p.xSpeed=(int)(-1*p.DASH_MAG/1.41421);
            p.ySpeed=(int)(-1*p.DASH_MAG/1.41421);
        }
        else if(keysPressed[RIGHT_KEY]){
            p.xSpeed=(int)(p.DASH_MAG/1.41421);
            p.ySpeed=(int)(-1*p.DASH_MAG/1.41421);
        }
        else{
            p.xSpeed=0;
            p.ySpeed=-p.DASH_MAG;
        }
    }
    else if(keysPressed[DOWN_KEY]){
        if(keysPressed[LEFT_KEY]){
            p.xSpeed=(int)(-1*p.DASH_MAG/1.41421);
            p.ySpeed=(int)(p.DASH_MAG/1.41421);
        }
        else if(keysPressed[RIGHT_KEY]){
            p.xSpeed=(int)(p.DASH_MAG/1.41421);
            p.ySpeed=(int)(p.DASH_MAG/1.41421);
        }
        else{
            p.xSpeed=0;
            p.ySpeed=p.DASH_MAG;
        }
    }
    else{
        if(p.facingRight){
            p.ySpeed=0;
            p.xSpeed=(int)p.DASH_MAG;
        }
        else{
            p.ySpeed=0;
            p.xSpeed=(int)-1*p.DASH_MAG;
        }
    }
}

bool toTheLeft(Person p,Platform plat){
    return plat.x-p.w-1==p.x&&((plat.y<p.y+p.h&&plat.y>p.y)||(plat.y+plat.h<p.y+p.h&&plat.y+plat.h>p.y)||(plat.y+plat.h<p.y+p.h&&plat.y>p.y)||(plat.y+plat.h>p.y+p.h&&plat.y<p.y));
}

bool toTheRight(Person p,Platform plat){
    return plat.x+plat.w+1==p.x&&((plat.y<p.y+p.h&&plat.y>p.y)||(plat.y+plat.h<p.y+p.h&&plat.y+plat.h>p.y)||(plat.y+plat.h<p.y+p.h&&plat.y>p.y)||(plat.y+plat.h>p.y+p.h&&plat.y<p.y));
}

void wallCase(Person &p,bool keysPressed[343],std::vector<Platform> platforms){
    if(p.againstWall!=-1){
        p.canJump=true;
        if(keysPressed[HOLD_KEY]){
            if(keysPressed[JUMP_KEY]&&p.canJump&&p.jumpReleased){
                p.canJump=false;
                p.jumpReleased=false;
                p.againstWall=-1;
                if(keysPressed[UP_KEY]){        // jump a good bit up
                    p.ySpeed=-p.JUMP_POWER;
                    if(p.wallRight){
                        p.xSpeed=-30;
                    }
                    else{
                        p.xSpeed=30;
                    }
                }
                else if(p.wallRight){
                    if(keysPressed[LEFT_KEY]){
                        p.xSpeed=-p.WALK_SPEED;
                        p.ySpeed=-p.JUMP_POWER;
                    }
                    else{
                        p.xSpeed=-p.WALK_SPEED;
                        p.ySpeed=-(p.JUMP_POWER/2);
                    }
                }
                else{
                    if(keysPressed[RIGHT_KEY]){
                        p.xSpeed=p.WALK_SPEED;
                        p.ySpeed=-p.JUMP_POWER;
                    }
                    else{
                        p.xSpeed=+p.WALK_SPEED;
                        p.ySpeed=-(p.JUMP_POWER/2);
                    }
                }
            }
            else if(keysPressed[UP_KEY]){
                p.ySpeed=-p.CLIMB_SPEED;
            }
            else if(keysPressed[DOWN_KEY]){
                p.ySpeed=p.CLIMB_SPEED;
            }
            else{
                p.ySpeed=0;
            }
        }
        else if(keysPressed[JUMP_KEY]&&p.jumpReleased){                 // PUNY JUMP
            p.againstWall=-1;
            p.canJump=false;
            p.jumpReleased=false;
            if(p.wallRight){
                p.xSpeed=-p.WALK_SPEED;
                p.ySpeed=-(p.JUMP_POWER/2);
            }
            else{
                p.xSpeed=+p.WALK_SPEED;
                p.ySpeed=-(p.JUMP_POWER/2);
            }
        }
        else if(p.wallRight&&keysPressed[LEFT_KEY]){
            p.againstWall=-1;
            p.xSpeed=-p.WALK_SPEED;
        }
        else if(!(p.wallRight)&&keysPressed[RIGHT_KEY]){
            p.againstWall=-1;
            p.xSpeed=p.WALK_SPEED;
        }
        else{
            p.ySpeed=p.SLIDE_SPEED;
        }
    }
    else{
        for(int i=0;i<platforms.size();i++){
            if(toTheLeft(p,platforms[i])){
                p.canJump=true;
                if(keysPressed[HOLD_KEY]||keysPressed[RIGHT_KEY]){                              // CLIMBING
                    p.againstWall=i;
                    p.wallRight=true;
                    p.ySpeed=0;
                    p.xSpeed=0;
                }
                else if(keysPressed[JUMP_KEY]&&keysPressed[LEFT_KEY]&&p.jumpReleased){  // QUICK WALL JUMP
                    p.canJump=false;
                    p.jumpReleased=false;
                    p.xSpeed=p.WALK_SPEED;
                    p.ySpeed=-p.JUMP_POWER;
                }
                break;
            }
            else if(toTheRight(p,platforms[i])){
                p.canJump=true;
                if(keysPressed[HOLD_KEY]||keysPressed[LEFT_KEY]){
                    p.againstWall=i;
                    p.wallRight=false;
                    p.ySpeed=0;
                    p.xSpeed=0;
                }
                else if(keysPressed[JUMP_KEY]&&keysPressed[RIGHT_KEY]&&p.jumpReleased){
                    p.canJump=false;
                    p.jumpReleased=false;
                    p.xSpeed=-p.WALK_SPEED;
                    p.ySpeed=-p.JUMP_POWER;
                }
                break;
            }
        }
    }
}

void leftRightConditions(Person &p,bool keysPressed[343]){
    if(keysPressed[LEFT_KEY]&&keysPressed[RIGHT_KEY]){
        if(p.xSpeed>0){
            p.xSpeed-=(p.WALK_SPEED/15);
            p.facingRight=false;
        }
        else if(p.xSpeed<0){
            p.xSpeed+=(p.WALK_SPEED/15);
            p.facingRight=true;
        }
    }
    else if(keysPressed[LEFT_KEY]){
        p.facingRight=false;
        if(p.xSpeed<=0){
            p.xSpeed=-p.WALK_SPEED;
        }
        else{
            p.xSpeed-=(p.WALK_SPEED/15);
        }
    }
    else if(keysPressed[RIGHT_KEY]){
        p.facingRight=true;
        if(p.xSpeed>=0){
            p.xSpeed=p.WALK_SPEED;
        }
        else{
            p.xSpeed+=(p.WALK_SPEED/15);
            p.facingRight=true;
        }
    }
    else{
        p.xSpeed=0;
    }
}

void checkInputs(Person &p,bool keysPressed[343],std::vector<Platform> platforms){         // checks the inputs

    if(keysPressed['r']){                                   // test cases
        SDL_Delay(3000);
    }
    if(!keysPressed[JUMP_KEY]){
        p.jumpReleased=true;
    }
    if(p.dashState==0&&keysPressed[DASH_KEY]){                 // This is all in the case that dash is pressed
        dashCase(p,keysPressed);
    }
    else if(p.dashState!=1){                                    // This is for normal movement
        if(p.againstWall==-1){
            leftRightConditions(p,keysPressed);
        }
        wallCase(p,keysPressed,platforms);
        if(p.againstWall==-1){
            if(keysPressed[JUMP_KEY]&&p.canJump&&p.jumpReleased){
                p.ySpeed=-p.JUMP_POWER;
                p.canJump=false;
                p.jumpReleased=false;
            }
            else if(keysPressed[DOWN_KEY]){
                p.ySpeed+=2;
            }
        }
    }
}
