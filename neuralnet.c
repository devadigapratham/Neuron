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
#define ARRAY_LEN(x) (sizeof(x) / sizeof(x[0]))
int arch[] = {2, 12, 25, 66, 4, 2, 1}; // You could customize here, plans to make it read from code and then visualize.

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
    c.r = 0x00;
    c.g = 0xFF;
    c.b = 0x00;
    c.a = 0xFF;

    float layers = ARRAY_LEN(arch);
    float layer_width = NETWORK_WIDTH / layers;
    float x_offset = layer_width / 2;

    for (int i = 0; i < layers; i++) {
        float xPos = x + i * layer_width + x_offset;

        for (int j = 0; j < arch[i]; j++) {
            float neuron_height = NETWORK_HEIGHT / arch[i];
            float y_offset = neuron_height / 2;
            float yPos = y + j * neuron_height + y_offset;

            if (i < layers - 1) {
                for (int k = 0; k < arch[i+1]; k++) {
                    float xPos2 = x + (i + 1) * layer_width + x_offset;
                    float neuron_height2 = NETWORK_HEIGHT / arch[i + 1];
                    float y_offset2 = neuron_height2 / 2;
                    float yPos2 = y + k * neuron_height2 + y_offset2;

                    SDL_SetRenderDrawColor(renderer, 0x55, 0x55, 0x55, 255);
                    SDL_RenderDrawLine(renderer, xPos, yPos, xPos2, yPos2);
                }
            }

            int size = 100 / arch[i] + 2;

            SDL_RenderFillCircle(renderer, xPos, yPos, size, c);
        }
    }

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
