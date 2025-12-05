#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdbool.h>
#include "linear_algebra_lib.h"
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
    struct vector_2d pixels_size_vector = {WIDTH, HEIGHT};
    float FOV_input = 60;
    float FOV = FOV_input * (M_PI / 180.0f);
    float smallest_screen_dimension = pixels_size_vector.x;
    
    int triangle_number = 5;
    struct vector_3d randomised_triangle_vectors[triangle_number * 3];
    fill_random_vectors_3d(randomised_triangle_vectors, triangle_number,
                           0, WIDTH - 1, 0, WIDTH - 1,0, WIDTH - 1);
    //struct world_vector starting_point = {250,250,250};
    //randomised_triangle_vectors = add_cube_to_world_vectors(randomised_triangle_vectors, triangle_number, starting_point, 1000);
    //triangle_number = triangle_number + 2;
    Uint32 randomised_colours[triangle_number];
    fill_random_colours(randomised_colours, triangle_number);
    log_triangle_vectors_3d(randomised_triangle_vectors, triangle_number);
    log_triangle_colours(randomised_colours, triangle_number);

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
        
        fill_random_vectors_3d(randomised_triangle_vectors, triangle_number,
                           0, WIDTH - 1, 0, WIDTH - 1,0, WIDTH - 1);
        // Fill the buffer with black 
        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                pixels[y * WIDTH + x] = (Uint32) (255 << 24) | (0 << 16) | (0 << 8) | 0;
            }
        }
        

        // Recolour any pixels that pass rasterising check to random colour
        for (int triangle_index = 0; triangle_index < triangle_number; triangle_index++) {
          struct vector_2d converted_triangle_vector_a = copy_vector_3d_to_2d(
                  randomised_triangle_vectors[triangle_index * 3 + 0]);

          struct vector_2d converted_triangle_vector_b = copy_vector_3d_to_2d(
                  randomised_triangle_vectors[triangle_index * 3 + 1]);

          struct vector_2d converted_triangle_vector_c = copy_vector_3d_to_2d(
                  randomised_triangle_vectors[triangle_index * 3 + 2]);
          
          
          int min_x = fmin(converted_triangle_vector_a.x, fmin(converted_triangle_vector_b.x, converted_triangle_vector_c.x));
          int max_x = fmax(converted_triangle_vector_a.x, fmin(converted_triangle_vector_b.x, converted_triangle_vector_b.x));
          int min_y = fmin(converted_triangle_vector_a.y, fmin(converted_triangle_vector_b.y, converted_triangle_vector_c.y));
          int max_y = fmax(converted_triangle_vector_a.y, fmax(converted_triangle_vector_b.y, converted_triangle_vector_c.y));
          
          for (int y = min_y; y < max_y; y++) {
              for (int x = min_x; x < max_x; x++) {
                struct vector_2d pixel_vector = {x, y};
                
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
