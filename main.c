#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdbool.h>
#include "matrix_math.h"
#define WIDTH 800  
#define HEIGHT 800

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("SDL init error: %s", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Custom Renderer",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          WIDTH, HEIGHT,
                                          0);
    if (!window) {
        SDL_Log("Window creation error: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    SDL_Texture *texture = SDL_CreateTexture(renderer,
                                             SDL_PIXELFORMAT_ARGB8888,
                                             SDL_TEXTUREACCESS_STREAMING,
                                             WIDTH, HEIGHT);

    bool running = true;
    SDL_Event event;

    // Example pixel buffer
    Uint32 *pixels = malloc(WIDTH * HEIGHT * sizeof(Uint32));
    struct screen_vector pixels_size_vector = {WIDTH, HEIGHT};
    float FOV_input = 60;
    float FOV = FOV_input * (M_PI / 180.0f);
    float smallest_screen_dimension = pixels_size_vector.x;
    
    int triangle_number = 5;
    struct world_vector* randomised_triangle_vectors = create_random_triangle_vectors(0, WIDTH - 1, 0, WIDTH - 1, 0, WIDTH - 1,
                                                                                      triangle_number);
    int* randomised_colours = create_random_triangle_colours(triangle_number);
    
    for (int i = 0; i < triangle_number; i++) {
      printf("First triangle vector:\n");
      log_world_vector(randomised_triangle_vectors[i * 3 + 0]);
      printf("  \n");
      printf("Second triangle vector:\n");
      log_world_vector(randomised_triangle_vectors[i * 3 + 1]);
      printf("  \n");
      printf("Third triangle vector:\n");
      log_world_vector(randomised_triangle_vectors[i * 3 + 2]);
      printf("  \n");
      printf("Triangle colour\n");
      log_colour(randomised_colours[i]);
      printf("=====\n");
    }

    while (running) {
        // Handle events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = false;
            else if (event.type == SDL_KEYDOWN) {
                const Uint8 *state = SDL_GetKeyboardState(NULL);
                // Meta (Super) + Q to quit
                if (state[SDL_SCANCODE_LGUI] && event.key.keysym.sym == SDLK_q)
                    running = false;
            }
        }
        
        // Fill the buffer with black 
        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                Uint8 r = 0; 
                Uint8 g = 0; 
                Uint8 b = 0; 
                pixels[y * WIDTH + x] = (255 << 24) | (r << 16) | (g << 8) | b;
            }
        }

        // Update texture with new pixels
        SDL_UpdateTexture(texture, NULL, pixels, WIDTH * sizeof(Uint32));

        // Render
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    free(pixels);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
