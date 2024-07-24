#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define FONT_SIZE 18
#define HEADER_FONT_SIZE 24
#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 800

// Function to render text
void render_text(SDL_Renderer *renderer, TTF_Font *font, const char *text, int x, int y, SDL_Color color) {
    SDL_Surface *textSurface = TTF_RenderText_Blended(font, text, color);
    if (!textSurface) {
        fprintf(stderr, "TTF_RenderText_Blended Error: %s\n", TTF_GetError());
        return;
    }

    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!textTexture) {
        fprintf(stderr, "SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
        SDL_FreeSurface(textSurface);
        return;
    }

    SDL_Rect textRect;
    textRect.x = x;
    textRect.y = y;
    textRect.w = textSurface->w;
    textRect.h = textSurface->h;

    SDL_FreeSurface(textSurface);

    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    SDL_DestroyTexture(textTexture);
}

// Function to render filled circle
void SDL_RenderFillCircle(SDL_Renderer *renderer, int x, int y, int radius, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
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

// Function to render the neural network
void render_network(SDL_Renderer *renderer, TTF_Font *font, const char *architecture_type, int x, int y) {
    SDL_Color layerColor = {0x00, 0x00, 0xFF, 255}; // Blue color for layer rectangles
    SDL_Color neuronColor = {0xFF, 0x00, 0x00, 255}; // Red color for neurons
    SDL_Color lineColor = {0x00, 0xFF, 0x00, 255}; // Green color for lines
    SDL_Color textColor = {255, 255, 255, 255}; // White color for text

    // Example architecture
    int layers[] = {64, 128, 10};
    int numLayers = sizeof(layers) / sizeof(layers[0]);
    float layerWidth = (float)(WINDOW_WIDTH - 100) / (numLayers + 1);
    float xOffset = layerWidth / 2;

    // Render the architecture type header
    render_text(renderer, font, architecture_type, x + 20, y - 40, textColor);

    // Render the layers
    for (int i = 0; i < numLayers; i++) {
        float xPos = x + (i + 1) * layerWidth;

        for (int j = 0; j < layers[i]; j++) {
            float neuronHeight = (float)(WINDOW_HEIGHT - 100) / layers[i];
            float yOffset = neuronHeight / 2;
            float yPos = y + j * neuronHeight + yOffset;

            if (i < numLayers - 1) {
                for (int k = 0; k < layers[i + 1]; k++) {
                    float xPos2 = x + (i + 2) * layerWidth;
                    float neuronHeight2 = (float)(WINDOW_HEIGHT - 100) / layers[i + 1];
                    float yOffset2 = neuronHeight2 / 2;
                    float yPos2 = y + k * neuronHeight2 + yOffset2;

                    SDL_SetRenderDrawColor(renderer, lineColor.r, lineColor.g, lineColor.b, lineColor.a);
                    SDL_RenderDrawLine(renderer, xPos, yPos, xPos2, yPos2);
                }
            }

            int size = 10;
            SDL_RenderFillCircle(renderer, xPos, yPos, size, neuronColor);
        }
    }

    // Add labels for layers
    render_text(renderer, font, "Input Layer", x + 10, y + 10, textColor);
    render_text(renderer, font, "Hidden Layer", x + (layerWidth / 2) - 50, y + (WINDOW_HEIGHT / 2) - 10, textColor);
    render_text(renderer, font, "Output Layer", x + (numLayers + 1) * layerWidth - 100, y + WINDOW_HEIGHT - 40, textColor);
}

int main() {
    SDL_Window *window;
    SDL_Renderer *renderer;
    TTF_Font *font;
    SDL_Event event;
    bool quit = false;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    if (TTF_Init() == -1) {
        fprintf(stderr, "TTF_Init Error: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    window = SDL_CreateWindow("Neuron - Network Visualization", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    font = TTF_OpenFont("DejaVuSans.ttf", FONT_SIZE);
    if (!font) {
        fprintf(stderr, "TTF_OpenFont Error: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
        SDL_RenderClear(renderer);

        // Render the network with a specified architecture type
        render_network(renderer, font, "Simple FeedForward Neural Network", 50, 50);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
