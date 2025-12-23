#include "vectors_and_matrices.c"
#include "screen_and_scene.c"


#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

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
    int maximum_triangle_amount = 40000;

    size_t pixels_buffer_size = sizeof(Uint32) * WIDTH * HEIGHT;
    size_t triangle_colours_size = sizeof(Uint32) * maximum_triangle_amount;
    size_t screen_vectors_size = sizeof(vector_2d) * maximum_triangle_amount * 3;
    size_t world_vectors_size = sizeof(vector_3d) * maximum_triangle_amount * 3;
    size_t MAXIMUM_SCREEN_MEMORY_REQUIREMENT = pixels_buffer_size + 
                                               screen_vectors_size + 
                                               world_vectors_size + 
                                               triangle_colours_size; 


    memory_arena screen_arena = {
      .capacity = MAXIMUM_SCREEN_MEMORY_REQUIREMENT,
      .memory = malloc(MAXIMUM_SCREEN_MEMORY_REQUIREMENT),
      .current_address_offset = 0,
    };

    Uint32 *pixels = allocate_arena_space(pixels_buffer_size, &screen_arena);
    Uint32 *triangle_colours = allocate_arena_space(triangle_colours_size, &screen_arena);
    vector_2d *screen_vectors = allocate_arena_space(screen_vectors_size, &screen_arena);
    vector_3d *world_vectors = allocate_arena_space(world_vectors_size, &screen_arena);


    vector_3d world_origin = {0, 0, 0};
    float FOV_input = 60;
    
    int triangle_number = 2;
     
    fill_random_vectors_3d(world_vectors, triangle_number,
                           0, WIDTH / 2- 1, 0, WIDTH / 2- 1,0, WIDTH / 2 - 1);

    vector_3d bottom_left_corner = {-7,-7,-30};
    fill_cube_vectors_3d(world_vectors, bottom_left_corner, 14);

     
    camera main_camera = {
    world_origin, 
    0, 0, 0,
    20, 40,
    FOV_input * (M_PI / 180.0f),
    HEIGHT / WIDTH
    };
      
    frustum_state camera_frustum_data = calculate_frustum_values(main_camera);
    // main_camera.yaw = M_PI / 12; 
    // main_camera.position = (vector_3d) {-700, -1000, -500};
    
    log_camera(main_camera);
    fill_random_colours(triangle_colours, triangle_number);
    log_triangle_vectors_3d(world_vectors, triangle_number);
    log_triangle_colours(triangle_colours, triangle_number);
    
    matrix_4x4 transformation_matrix_buffer[TOTAL_MATRIX_COUNT];

    int matrix_initial_states[] =  {1, 1, 1, 1, 1, 1, 1, 1, 1};

    initialise_matrix_buffer(transformation_matrix_buffer, matrix_initial_states);


    update_z_rotation_matrix(&transformation_matrix_buffer[Z_ROTATION_MATRIX], main_camera.yaw);
    printf("%d \n", Z_ROTATION_MATRIX);
    update_translation_matrix(&transformation_matrix_buffer[POSITION_TRANSLATION_MATRIX], main_camera.position);
    update_projection_matrix(&transformation_matrix_buffer[PROJECTION_MATRIX], camera_frustum_data);
    
    calculate_mapping_matrix(transformation_matrix_buffer);
    log_matrix_4x4(transformation_matrix_buffer[8]);
    map_world_space_vectors_to_screen_coordinates(
    world_vectors, screen_vectors, 
    &transformation_matrix_buffer[FINAL_MAPPING_MATRIX], triangle_number,
    WIDTH, HEIGHT);

    printf("new vectors\n"); 
    log_triangle_vectors_2d(screen_vectors, triangle_number);

    const float MOVE_SPEED  = 0.1f;
    const float ANGLE_SPEED = 5.0f * (M_PI / 180.0f);  // degrees per frame (or rad, your choice
    struct timespec start, end;  
    // running = false; 
    while (running) {
        clock_gettime(CLOCK_MONOTONIC, &start);


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


        // --- Get keyboard state for smooth movement ---
        const Uint8* keys = SDL_GetKeyboardState(NULL);
         

        // Movement: W A S D
        if (keys[SDL_SCANCODE_W]) {
          main_camera.position.z -= MOVE_SPEED;
          update_translation_matrix(&transformation_matrix_buffer[POSITION_TRANSLATION_MATRIX], main_camera.position);
        }

        if (keys[SDL_SCANCODE_S]) {
           main_camera.position.z += MOVE_SPEED;
           update_translation_matrix(&transformation_matrix_buffer[POSITION_TRANSLATION_MATRIX], main_camera.position);
        }

        if (keys[SDL_SCANCODE_A]) {
          main_camera.position.x -= MOVE_SPEED;
          update_translation_matrix(&transformation_matrix_buffer[POSITION_TRANSLATION_MATRIX], main_camera.position);
        }

        if (keys[SDL_SCANCODE_D]) {
          main_camera.position.x += MOVE_SPEED;
          update_translation_matrix(&transformation_matrix_buffer[POSITION_TRANSLATION_MATRIX], main_camera.position);
        }

        // moving up and down 
        if (keys[SDL_SCANCODE_E]) {
          main_camera.position.y -= MOVE_SPEED;
          update_translation_matrix(&transformation_matrix_buffer[POSITION_TRANSLATION_MATRIX], main_camera.position);
        }

        if (keys[SDL_SCANCODE_R]) {
          main_camera.position.y += MOVE_SPEED;
          update_translation_matrix(&transformation_matrix_buffer[POSITION_TRANSLATION_MATRIX], main_camera.position);
        }
        
        // Angles: H J K
        // You can decide the meaning (e.g., yaw / pitch / roll)
        if (keys[SDL_SCANCODE_H] && !keys[SDL_SCANCODE_LSHIFT]) {
          main_camera.yaw = fmodf((main_camera.yaw - ANGLE_SPEED), (-2.0f * (float)M_PI));
          update_x_rotation_matrix(&transformation_matrix_buffer[Z_ROTATION_MATRIX], main_camera.yaw);
        }

        if (keys[SDL_SCANCODE_H] && keys[SDL_SCANCODE_LSHIFT]) {
          main_camera.yaw = fmodf((main_camera.yaw + ANGLE_SPEED), (2.0f * (float)M_PI));
          update_x_rotation_matrix(&transformation_matrix_buffer[Z_ROTATION_MATRIX], main_camera.yaw);
        }


        if (keys[SDL_SCANCODE_J] && !keys[SDL_SCANCODE_LSHIFT]) {
          main_camera.pitch = fmodf((main_camera.pitch - ANGLE_SPEED), (-2.0f * (float)M_PI));
          update_y_rotation_matrix(&transformation_matrix_buffer[Y_ROTATION_MATRIX], main_camera.pitch);
        }

        if (keys[SDL_SCANCODE_J] && keys[SDL_SCANCODE_LSHIFT]) {
          main_camera.pitch = fmodf((main_camera.pitch + ANGLE_SPEED), (2.0f * (float)M_PI));
          update_y_rotation_matrix(&transformation_matrix_buffer[Y_ROTATION_MATRIX], main_camera.pitch);
        }


        if (keys[SDL_SCANCODE_K] && !keys[SDL_SCANCODE_LSHIFT]) {
          main_camera.roll = fmodf((main_camera.roll - ANGLE_SPEED), (-2.0f * (float)M_PI));
          update_z_rotation_matrix(&transformation_matrix_buffer[X_ROTATION_MATRIX], main_camera.roll);
        }

        if (keys[SDL_SCANCODE_K] && keys[SDL_SCANCODE_LSHIFT]) {
          main_camera.roll = fmodf((main_camera.roll + ANGLE_SPEED), (2.0f * (float)M_PI));
          update_z_rotation_matrix(&transformation_matrix_buffer[X_ROTATION_MATRIX], main_camera.roll);
        }

        calculate_mapping_matrix(transformation_matrix_buffer);

        map_world_space_vectors_to_screen_coordinates(
          world_vectors, screen_vectors, 
          &transformation_matrix_buffer[FINAL_MAPPING_MATRIX], triangle_number,
        WIDTH, HEIGHT);

        // For debugging
        printf("pos: (%f, %f, %f) | yaw: %f pitch: %f roll: %f\n",
            main_camera.position.x, main_camera.position.y, main_camera.position.z, 
            main_camera.yaw , main_camera.pitch, main_camera.roll);

        // Fill the buffer with black 
        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                pixels[y * WIDTH + x] = (Uint32) ((Uint32) 255 << 24) | (0 << 16) | (0 << 8) | 0;
            }
        }
        

        // Recolour any pixels that pass rasterising check to random colour
        rasterise_screen_coordinates(screen_vectors, triangle_number, triangle_colours, pixels, WIDTH);

        // Update texture with new pixels
        SDL_UpdateTexture(texture, NULL, pixels, WIDTH * sizeof(Uint32));

        // Render
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);

        clock_gettime(CLOCK_MONOTONIC, &end);

        // Calculate time in milliseconds
        double elapsed_ms = (end.tv_sec - start.tv_sec) * 1000.0 + 
                           (end.tv_nsec - start.tv_nsec) / 1000000.0;

        printf("Frame Time: %.2f ms\n", elapsed_ms);       // Handle events
    }

    free(screen_arena.memory);

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
