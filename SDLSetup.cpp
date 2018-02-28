#include <SDL.h>
#include <stdio.h>

bool init(SDL_Window* &w, SDL_Renderer* &r,SDL_Surface* &ss,int sw,int sh){            // Initializes SDL
    if(SDL_Init(SDL_INIT_VIDEO)<0){
        printf("SDL Can't Initialize: \n",SDL_GetError());
        return false;
    }
    w=SDL_CreateWindow("Bouncy Bounce",100,100,sw,sh,SDL_WINDOW_SHOWN);
    if(w==NULL){
        printf("Window cannot be created");
        return false;
    }
    ss=SDL_GetWindowSurface(w);
    r=SDL_CreateRenderer(w,-1,SDL_RENDERER_ACCELERATED);
    if(r==NULL){
        printf("Rendered cannot be made!");
        return false;
    }
    SDL_SetRenderDrawColor(r,0x00,0x00,0x00,0xFF);
    return true;
}

void quit(SDL_Window* &w){                // closes SDL
    SDL_DestroyWindow(w);
    SDL_Quit();
}



