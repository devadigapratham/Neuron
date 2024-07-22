#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>
#include <stdio.h>
#include "neuralnet.h"

#define WINDOW_X 10
#define WINDOW_Y -20
#define WINDOW_WIDTH 1770
#define WINDOW_HEIGHT 1410
#define NETWORK_WIDTH 1600
#define NETWORK_HEIGHT 1300

void SDL_RenderFillCircle(SDL_Renderer *renderer, int x, int y, int radius, SDL_Color c) {

    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
    for(int w = 0; w < radius * 2; w++) {
        for(int h = 0; h < radius * 2; h++) {
            int dx = radius - w;
            int dy = radius - h;
            if((dx * dx + dy * dy) < (radius * radius)) {
                SDL_RenderDrawPoint(renderer, x + dx, y + dy);
            }
        }
    }
    return;
}

void render_network(SDL_Renderer *renderer, int x, int y) {
    SDL_SetRenderDrawColor(renderer, 0x55, 0x55, 0x55, 255);

    SDL_Rect box;
    box.w = NETWORK_WIDTH;
    box.h = NETWORK_HEIGHT;
    box.x = x;
    box.y = y;

    SDL_RenderDrawRect(renderer, &box);
    SDL_Color c;
    c.r = 0xFF;
    c.g = 0x00;
    c.b = 0x00;
    c.a = 0xFF;

    SDL_RenderFillCircle(renderer, x + 200, y + 200, 150, c);

    return;
}

int main() {
    SDL_Window *window;
    SDL_Renderer *renderer;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "ERROR : SDL_Init: %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow(
        "",
        WINDOW_X,
        WINDOW_Y,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_BORDERLESS
    );

    if (!window) {
        fprintf(stderr, "Error : %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    renderer = SDL_CreateRenderer(
        window,
        -1 ,
        SDL_RENDERER_ACCELERATED
    );

    if(!renderer) {
        fprintf(stderr, "ERROR : !renderer");
    }

    int net_x = (WINDOW_WIDTH / 2) - (NETWORK_WIDTH / 2);
    int net_y = (WINDOW_HEIGHT / 2) - (NETWORK_HEIGHT / 2);

    SDL_Event event;

    bool quit = false;

    while(!quit) {
        while(SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_KEYUP:
                    break;
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                            quit = true;
                            break;
                    }
                break;
            }
        }
        SDL_RenderClear(renderer);
        //Starting RENDER LOOP:

        render_network(renderer, net_x, net_y);

        //END of RENDER LOOP!
        SDL_SetRenderDrawColor(renderer, 0x11, 0x11, 0x11, 255);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
