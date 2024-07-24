// #include "raylib.h"
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <cjson/cJSON.h>
// #include "raymath.h" // Include for Vector2 functions

// #define SCREEN_WIDTH 1800
// #define SCREEN_HEIGHT 900
// #define NODE_RADIUS 30
// #define LINE_THICKNESS 3
// #define HEADING_SIZE 40
// #define NODE_COLOR BLUE
// #define CONNECTION_COLOR DARKGRAY
// #define HEADING_COLOR DARKBLUE
// #define INTERACTIVE_COLOR LIGHTGRAY
// #define FONT_SIZE 20

// typedef struct {
//     int units;
//     char activation[10];
//     char name[50];
// } Layer;

// // Load architecture from JSON file
// Layer* LoadArchitecture(const char* filename, int* num_layers) {
//     FILE* file = fopen(filename, "r");
//     if (!file) {
//         fprintf(stderr, "Failed to open JSON file.\n");
//         exit(1);
//     }

//     fseek(file, 0, SEEK_END);
//     long length = ftell(file);
//     fseek(file, 0, SEEK_SET);

//     char* content = (char*)malloc(length + 1);
//     fread(content, 1, length, file);
//     fclose(file);
//     content[length] = '\0';

//     cJSON* json = cJSON_Parse(content);
//     if (!json) {
//         fprintf(stderr, "Failed to parse JSON file.\n");
//         free(content);
//         exit(1);
//     }

//     // Ensure JSON is an array
//     if (!cJSON_IsArray(json)) {
//         fprintf(stderr, "JSON data is not an array.\n");
//         cJSON_Delete(json);
//         free(content);
//         exit(1);
//     }

//     *num_layers = cJSON_GetArraySize(json);
//     Layer* layer_array = (Layer*)malloc(sizeof(Layer) * (*num_layers));

//     for (int i = 0; i < *num_layers; i++) {
//         cJSON* layer = cJSON_GetArrayItem(json, i);
//         cJSON* units = cJSON_GetObjectItem(layer, "units");
//         cJSON* activation = cJSON_GetObjectItem(layer, "activation");
//         cJSON* name = cJSON_GetObjectItem(layer, "name");

//         if (units && activation && name) {
//             layer_array[i].units = units->valueint;
//             strncpy(layer_array[i].activation, activation->valuestring, sizeof(layer_array[i].activation) - 1);
//             layer_array[i].activation[sizeof(layer_array[i].activation) - 1] = '\0';
//             strncpy(layer_array[i].name, name->valuestring, sizeof(layer_array[i].name) - 1);
//             layer_array[i].name[sizeof(layer_array[i].name) - 1] = '\0';
//         } else {
//             fprintf(stderr, "Invalid JSON structure.\n");
//             cJSON_Delete(json);
//             free(content);
//             free(layer_array);
//             exit(1);
//         }
//     }

//     cJSON_Delete(json);
//     free(content);
//     return layer_array;
// }

// // Draw the neural network
// void DrawNeuralNetwork(Layer* layers, int num_layers) {
//     float layer_width = (float)SCREEN_WIDTH / (num_layers + 1);
//     float max_layer_height = SCREEN_HEIGHT - 2 * NODE_RADIUS; // Space for top and bottom padding

//     for (int i = 0; i < num_layers; i++) {
//         float xPos = layer_width * (i + 1);
//         float y_offset = max_layer_height / layers[i].units;
//         float startY = NODE_RADIUS + (max_layer_height - (layers[i].units - 1) * y_offset) / 2;

//         for (int j = 0; j < layers[i].units; j++) {
//             float yPos = startY + j * y_offset;

//             // Check for interaction
//             Vector2 mousePos = GetMousePosition();
//             bool isHovered = (Vector2Distance(mousePos, (Vector2){xPos, yPos}) < NODE_RADIUS);
//             Color nodeColor = isHovered ? INTERACTIVE_COLOR : NODE_COLOR;

//             // Draw neurons
//             DrawCircle((int)xPos, (int)yPos, NODE_RADIUS, nodeColor);
//             DrawText(TextFormat("%d", layers[i].units), xPos + NODE_RADIUS / 2, yPos - NODE_RADIUS / 2, FONT_SIZE, WHITE);

//             // Draw connections
//             if (i < num_layers - 1) {
//                 float nextXPos = layer_width * (i + 2);
//                 float nextY_offset = max_layer_height / layers[i + 1].units;
//                 float nextStartY = NODE_RADIUS + (max_layer_height - (layers[i + 1].units - 1) * nextY_offset) / 2;

//                 for (int k = 0; k < layers[i + 1].units; k++) {
//                     float nextYPos = nextStartY + k * nextY_offset;
//                     DrawLine((int)xPos, (int)yPos, (int)nextXPos, (int)nextYPos, CONNECTION_COLOR);
//                 }
//             }
//         }
//     }
// }

// // Draw the heading text
// void DrawHeading(const char* title) {
//     int textWidth = MeasureText(title, HEADING_SIZE);
//     DrawText(title, (SCREEN_WIDTH - textWidth) / 2, 20, HEADING_SIZE, HEADING_COLOR);
// }

// // Main function
// int main(void) {
//     int num_layers;
//     Layer* layers = LoadArchitecture("architecture.json", &num_layers);

//     InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Neural Network Visualization");
//     SetTargetFPS(60);

//     while (!WindowShouldClose()) {
//         BeginDrawing();
//         ClearBackground(RAYWHITE);

//         DrawHeading("Neural Network Architecture Visualization");
//         DrawNeuralNetwork(layers, num_layers);

//         EndDrawing();
//     }

//     free(layers);
//     CloseWindow();

//     return 0;
// }



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>        // For fmaxf
#include <raymath.h>     // Include raymath before raylib
#include <raylib.h>
#include <cjson/cJSON.h>

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 800
#define NODE_RADIUS 30
#define HEADING_SIZE 40
#define FONT_SIZE 20

typedef struct {
    int units;
    char activation[20];
    char name[50];
} Layer;

void DrawNeuralNetwork(Layer *layer_array, int num_layers) {
    ClearBackground(RAYWHITE);

    // Draw heading
    const char *heading = "Neural Network Visualization";
    int headingWidth = MeasureText(heading, HEADING_SIZE);
    DrawText(heading, (SCREEN_WIDTH - headingWidth) / 2, 20, HEADING_SIZE, BLACK);

    float layer_spacing = (SCREEN_WIDTH - 100) / (num_layers + 1);
    float layer_x = 50;

    // Calculate max height of layers
    float max_height = 0;
    for (int i = 0; i < num_layers; i++) {
        max_height = fmaxf(max_height, layer_array[i].units * NODE_RADIUS * 2.5);
    }

    // Center layers vertically
    float start_y = SCREEN_HEIGHT / 2 - max_height / 2;

    for (int i = 0; i < num_layers; i++) {
        float neuron_spacing = max_height / (layer_array[i].units + 1);
        float neuron_y = start_y;

        // Draw connections
        if (i < num_layers - 1) {
            for (int j = 0; j < layer_array[i].units; j++) {
                for (int k = 0; k < layer_array[i + 1].units; k++) {
                    Vector2 start = { layer_x + NODE_RADIUS, neuron_y + j * neuron_spacing };
                    Vector2 end = { layer_x + layer_spacing - NODE_RADIUS, start_y + k * neuron_spacing };
                    DrawLineV(start, end, GRAY);
                }
            }
        }

        // Draw neurons
        for (int j = 0; j < layer_array[i].units; j++) {
            float xPos = layer_x + NODE_RADIUS;
            float yPos = neuron_y + j * neuron_spacing;
            Vector2 position = { xPos, yPos };

            // Check for mouse hover
            Vector2 mousePos = GetMousePosition();
            bool isHovered = (Vector2Distance(mousePos, position) < NODE_RADIUS);

            // Draw node
            Color nodeColor = isHovered ? RED : DARKGRAY;
            DrawCircleV(position, NODE_RADIUS, nodeColor);

            // Draw label
            if (isHovered) {
                char label[100];
                snprintf(label, sizeof(label), "%s\nActivation: %s", layer_array[i].name, layer_array[i].activation);
                int labelWidth = MeasureText(label, FONT_SIZE);
                DrawRectangleLines(xPos - NODE_RADIUS - 5, yPos - NODE_RADIUS - FONT_SIZE - 10, labelWidth + 10, FONT_SIZE * 2, BLACK);
                DrawText(label, xPos - NODE_RADIUS - 5, yPos - NODE_RADIUS - FONT_SIZE - 5, FONT_SIZE, BLACK);
            }
        }

        layer_x += layer_spacing;
    }
}

Layer* LoadArchitecture(const char *filename, int *num_layers) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Could not open file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *json_string = (char *)malloc(file_size + 1);
    fread(json_string, 1, file_size, file);
    json_string[file_size] = '\0';
    fclose(file);

    cJSON *json = cJSON_Parse(json_string);
    free(json_string);

    if (!json) {
        fprintf(stderr, "Error parsing JSON: %s\n", cJSON_GetErrorPtr());
        exit(EXIT_FAILURE);
    }

    cJSON *layers = cJSON_GetObjectItemCaseSensitive(json, "layers");
    if (!cJSON_IsArray(layers)) {
        fprintf(stderr, "JSON 'layers' is not an array.\n");
        cJSON_Delete(json);
        exit(EXIT_FAILURE);
    }

    *num_layers = cJSON_GetArraySize(layers);
    Layer *layer_array = (Layer *)malloc(*num_layers * sizeof(Layer));

    for (int i = 0; i < *num_layers; i++) {
        cJSON *layer = cJSON_GetArrayItem(layers, i);
        if (!cJSON_IsObject(layer)) {
            fprintf(stderr, "Invalid layer format.\n");
            free(layer_array);
            cJSON_Delete(json);
            exit(EXIT_FAILURE);
        }

        cJSON *units = cJSON_GetObjectItemCaseSensitive(layer, "units");
        cJSON *activation = cJSON_GetObjectItemCaseSensitive(layer, "activation");
        cJSON *name = cJSON_GetObjectItemCaseSensitive(layer, "name");

        if (!cJSON_IsNumber(units) || !cJSON_IsString(activation) || !cJSON_IsString(name)) {
            fprintf(stderr, "Invalid data types in JSON.\n");
            free(layer_array);
            cJSON_Delete(json);
            exit(EXIT_FAILURE);
        }

        layer_array[i].units = units->valueint;
        snprintf(layer_array[i].activation, sizeof(layer_array[i].activation), "%s", activation->valuestring);
        snprintf(layer_array[i].name, sizeof(layer_array[i].name), "%s", name->valuestring);
    }

    cJSON_Delete(json);
    return layer_array;
}

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Neural Network Visualization");
    SetTargetFPS(60);

    int num_layers;
    Layer *layer_array = LoadArchitecture("architecture.json", &num_layers);

    while (!WindowShouldClose()) {
        BeginDrawing();
        DrawNeuralNetwork(layer_array, num_layers);
        EndDrawing();
    }

    free(layer_array);
    CloseWindow();
    return 0;
}
