#include <SDL2/SDL.h>
#include <jansson.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define WINDOW_X 10
#define WINDOW_Y -20
#define WINDOW_WIDTH 1770
#define WINDOW_HEIGHT 1010
#define NETWORK_WIDTH 1600
#define NETWORK_HEIGHT 1000
#define ARRAY_LEN(x) (sizeof(x) / sizeof(x[0]))

// Define the neural network architecture structure
typedef struct {
    int num_layers;
    int *layer_sizes;
} NeuralNetwork;

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
}

NeuralNetwork* parse_architecture(const char *filename) {
    json_error_t error;
    json_t *root = json_load_file(filename, 0, &error);
    if (!root) {
        fprintf(stderr, "Error loading JSON file: %s\n", error.text);
        return NULL;
    }

    json_t *layers = json_object_get(root, "config");
    json_t *layer_list = json_object_get(layers, "layers");
    size_t num_layers = json_array_size(layer_list);

    NeuralNetwork *nn = malloc(sizeof(NeuralNetwork));
    nn->num_layers = num_layers;
    nn->layer_sizes = malloc(num_layers * sizeof(int));

    for (size_t i = 0; i < num_layers; i++) {
        json_t *layer = json_array_get(layer_list, i);
        json_t *config = json_object_get(layer, "config");
        int units = json_integer_value(json_object_get(config, "units"));
        nn->layer_sizes[i] = units;
    }

    json_decref(root);
    return nn;
}

void render_network(SDL_Renderer *renderer, int x, int y, NeuralNetwork *nn) {
    SDL_SetRenderDrawColor(renderer, 0x55, 0x55, 0x55, 255);

    SDL_Rect box;
    box.w = NETWORK_WIDTH;
    box.h = NETWORK_HEIGHT;
    box.x = x;
    box.y = y;
    SDL_RenderDrawRect(renderer, &box);

    SDL_Color c = {0x00, 0xFF, 0x00, 0xFF};

    float layers = nn->num_layers;
    float layer_width = NETWORK_WIDTH / layers;
    float x_offset = layer_width / 2;

    for (int i = 0; i < nn->num_layers; i++) {
        float xPos = x + i * layer_width + x_offset;

        for (int j = 0; j < nn->layer_sizes[i]; j++) {
            float neuron_height = NETWORK_HEIGHT / nn->layer_sizes[i];
            float y_offset = neuron_height / 2;
            float yPos = y + j * neuron_height + y_offset;

            if (i < nn->num_layers - 1) {
                for (int k = 0; k < nn->layer_sizes[i + 1]; k++) {
                    float xPos2 = x + (i + 1) * layer_width + x_offset;
                    float neuron_height2 = NETWORK_HEIGHT / nn->layer_sizes[i + 1];
                    float y_offset2 = neuron_height2 / 2;
                    float yPos2 = y + k * neuron_height2 + y_offset2;

                    SDL_SetRenderDrawColor(renderer, 0x55, 0x55, 0x55, 255);
                    SDL_RenderDrawLine(renderer, xPos, yPos, xPos2, yPos2);
                }
            }

            int size = 100 / nn->layer_sizes[i] + 2;
            SDL_RenderFillCircle(renderer, xPos, yPos, size, c);
        }
    }
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
        -1,
        SDL_RENDERER_ACCELERATED
    );

    if (!renderer) {
        fprintf(stderr, "ERROR : !renderer\n");
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    NeuralNetwork *nn = parse_architecture("architecture.json");
    if (!nn) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    int net_x = (WINDOW_WIDTH / 2) - (NETWORK_WIDTH / 2);
    int net_y = (WINDOW_HEIGHT / 2) - (NETWORK_HEIGHT / 2);

    SDL_Event event;
    bool quit = false;

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        quit = true;
                    }
                    break;
            }
        }
        SDL_RenderClear(renderer);
        render_network(renderer, net_x, net_y, nn);
        SDL_SetRenderDrawColor(renderer, 0x11, 0x11, 0x11, 255);
        SDL_RenderPresent(renderer);
    }

    free(nn->layer_sizes);
    free(nn);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
