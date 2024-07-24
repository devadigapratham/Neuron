#include <SDL2/SDL.h>
#include <stdbool.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>

#define WINDOW_X 10
#define WINDOW_Y -20
#define WINDOW_WIDTH 1770
#define WINDOW_HEIGHT 1010
#define NETWORK_WIDTH 1600
#define NETWORK_HEIGHT 1000
#define FONT_SIZE 20

// Function prototypes
void SDL_RenderFillCircle(SDL_Renderer *renderer, int x, int y, int radius, SDL_Color c);
void SDL_RenderDrawText(SDL_Renderer *renderer, TTF_Font *font, int x, int y, const char *text, SDL_Color color);
void render_network(SDL_Renderer *renderer, TTF_Font *font, int x, int y, json_t *json);

int main() {
    SDL_Window *window;
    SDL_Renderer *renderer;
    TTF_Font *font;
    SDL_Event event;
    bool quit = false;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "ERROR : SDL_Init: %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow(
        "Neuron Network Visualization",
        WINDOW_X,
        WINDOW_Y,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        fprintf(stderr, "Error : SDL_CreateWindow: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(stderr, "ERROR : SDL_CreateRenderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    if (TTF_Init() == -1) {
        fprintf(stderr, "ERROR : TTF_Init: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    font = TTF_OpenFont("DejaVuSans.ttf", FONT_SIZE);
    if (!font) {
        fprintf(stderr, "ERROR : TTF_OpenFont: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Load JSON file
    json_error_t error;
    json_t *root = json_load_file("architecture.json", 0, &error);
    if (!root) {
        fprintf(stderr, "ERROR : Could not load JSON file: %s\n", error.text);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_CloseFont(font);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    int net_x = (WINDOW_WIDTH / 2) - (NETWORK_WIDTH / 2);
    int net_y = (WINDOW_HEIGHT / 2) - (NETWORK_HEIGHT / 2);

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                quit = true;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0x11, 0x11, 0x11, 255);
        SDL_RenderClear(renderer);

        render_network(renderer, font, net_x, net_y, root);

        SDL_RenderPresent(renderer);
    }

    json_decref(root);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

void SDL_RenderFillCircle(SDL_Renderer *renderer, int x, int y, int radius, SDL_Color c) {
    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            int dx = radius - w;
            int dy = radius - h;
            if ((dx * dx + dy * dy) < (radius * radius)) {
                SDL_RenderDrawPoint(renderer, x + dx, y + dy);
            }
        }
    }
}

void SDL_RenderDrawText(SDL_Renderer *renderer, TTF_Font *font, int x, int y, const char *text, SDL_Color color) {
    SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);
    if (!surface) {
        fprintf(stderr, "ERROR : TTF_RenderText_Solid: %s\n", TTF_GetError());
        return;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        fprintf(stderr, "ERROR : SDL_CreateTextureFromSurface: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        return;
    }
    SDL_Rect dstrect = { x, y, surface->w, surface->h };
    SDL_RenderCopy(renderer, texture, NULL, &dstrect);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

void render_network(SDL_Renderer *renderer, TTF_Font *font, int x, int y, json_t *json) {
    SDL_SetRenderDrawColor(renderer, 0x55, 0x55, 0x55, 255);
    SDL_Rect box = { x, y, NETWORK_WIDTH, NETWORK_HEIGHT };
    SDL_RenderDrawRect(renderer, &box);

    SDL_Color neuronColor = { 0x00, 0xFF, 0x00, 0xFF };
    SDL_Color textColor = { 0xFF, 0xFF, 0xFF, 0xFF };

    size_t i, j, k;
    size_t layers = json_array_size(json);
    float layer_width = NETWORK_WIDTH / layers;
    float x_offset = layer_width / 2;

    for (i = 0; i < layers; i++) {
        json_t *layer = json_array_get(json, i);
        size_t num_neurons = json_integer_value(json_object_get(layer, "units"));
        float xPos = x + i * layer_width + x_offset;

        for (j = 0; j < num_neurons; j++) {
            float neuron_height = NETWORK_HEIGHT / num_neurons;
            float y_offset = neuron_height / 2;
            float yPos = y + j * neuron_height + y_offset;

            if (i < layers - 1) {
                json_t *next_layer = json_array_get(json, i + 1);
                size_t next_layer_neurons = json_integer_value(json_object_get(next_layer, "units"));
                float xPos2 = x + (i + 1) * layer_width + x_offset;

                for (k = 0; k < next_layer_neurons; k++) {
                    float neuron_height2 = NETWORK_HEIGHT / next_layer_neurons;
                    float y_offset2 = neuron_height2 / 2;
                    float yPos2 = y + k * neuron_height2 + y_offset2;

                    SDL_SetRenderDrawColor(renderer, 0x55, 0x55, 0x55, 255);
                    SDL_RenderDrawLine(renderer, xPos, yPos, xPos2, yPos2);
                }
            }

            int size = 100 / num_neurons + 2;
            SDL_RenderFillCircle(renderer, xPos, yPos, size, neuronColor);

            char label[100];
            snprintf(label, sizeof(label), "Neuron %zu", j + 1);
            SDL_RenderDrawText(renderer, font, xPos + size, yPos - FONT_SIZE / 2, label, textColor);
        }
    }
}
