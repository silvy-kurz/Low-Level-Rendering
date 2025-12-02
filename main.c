#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdbool.h>
#include "matrix_math.h"
#define WIDTH 2000  
#define HEIGHT 2000 

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
    
    int triangle_number = 10;
    struct world_vector* randomised_triangle_vectors = create_random_triangle_vectors(0, WIDTH - 1, 0, WIDTH - 1, 0, WIDTH - 1,
                                                                                      triangle_number);
    int* randomised_colours = create_random_triangle_colours(triangle_number);
    

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
                pixels[y * WIDTH + x] = (255 << 24) | (0 << 16) | (0 << 8) | 0;
            }
        }
        

        // Recolour any pixels that pass rasterising check to random colour
        for (int triangle_index = 0; triangle_index < triangle_number; triangle_index++) {
          struct screen_vector converted_triangle_vector_a = {
            randomised_triangle_vectors[triangle_index * 3 + 0].x, 
            randomised_triangle_vectors[triangle_index * 3 + 0].y};
          
          struct screen_vector converted_triangle_vector_b = {
            randomised_triangle_vectors[triangle_index * 3 + 1].x, 
            randomised_triangle_vectors[triangle_index * 3 + 1].y};

          struct screen_vector converted_triangle_vector_c = {
            randomised_triangle_vectors[triangle_index * 3 + 2].x, 
            randomised_triangle_vectors[triangle_index * 3 + 2].y};
          
          int min_x = fmin(converted_triangle_vector_a.x, fmin(converted_triangle_vector_b.x, converted_triangle_vector_c.x));
          int max_x = fmax(converted_triangle_vector_a.x, fmin(converted_triangle_vector_b.x, converted_triangle_vector_b.x));
          int min_y = fmin(converted_triangle_vector_a.y, fmin(converted_triangle_vector_b.y, converted_triangle_vector_c.y));
          int max_y = fmax(converted_triangle_vector_a.y, fmax(converted_triangle_vector_b.y, converted_triangle_vector_c.y));
          
          for (int y = min_y; y < max_y; y++) {
              for (int x = min_x; x < max_x; x++) {
                struct screen_vector pixel_vector = {x, y};
                
                if (is_point_in_triangle(
                    converted_triangle_vector_a, 
                    converted_triangle_vector_b, 
                    converted_triangle_vector_c, 
                    pixel_vector)) {
                  pixels[y * WIDTH + x] = randomised_colours[triangle_index];
                }
              }
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
