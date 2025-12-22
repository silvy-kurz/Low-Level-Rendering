#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdbool.h>
#include "vectors_and_matrices.c"
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
    vector_3d world_origin = {0, 0, 0};
    vector_2d pixels_size_vector = {WIDTH, HEIGHT};
    float FOV_input = 60;
    float FOV = FOV_input * (M_PI / 180.0f);
    float smallest_screen_dimension = pixels_size_vector.x;
    
    int triangle_number = 2;
    vector_3d randomised_triangle_vectors[triangle_number * 3];
    vector_2d screen_vectors[triangle_number * 3];
    fill_random_vectors_3d(randomised_triangle_vectors, triangle_number,
                           0, WIDTH / 2- 1, 0, WIDTH / 2- 1,0, WIDTH / 2 - 1);

    vector_3d bottom_left_corner = {-7,-7,-30};
    fill_cube_vectors_3d(randomised_triangle_vectors, bottom_left_corner, 14);
    Uint32 randomised_colours[triangle_number];

     
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
    fill_random_colours(randomised_colours, triangle_number);
    log_triangle_vectors_3d(randomised_triangle_vectors, triangle_number);
    log_triangle_colours(randomised_colours, triangle_number);
    
    int transformation_matrices_amount = 9;
    matrix_4x4 transformation_matrix_buffer[transformation_matrices_amount];

    int matrix_initial_states[] =  {1, 1, 1, 1, 1, 1, 1, 1, 1};

    initialise_matrix_buffer(transformation_matrix_buffer, transformation_matrices_amount, matrix_initial_states);


    update_z_rotation_matrix(&transformation_matrix_buffer[Z_ROTATION_MATRIX], main_camera.yaw);
    printf("%d \n", Z_ROTATION_MATRIX);
    update_translation_matrix(&transformation_matrix_buffer[POSITION_TRANSLATION_MATRIX], main_camera.position);
    update_projection_matrix(&transformation_matrix_buffer[PROJECTION_MATRIX], camera_frustum_data);
    
    calculate_mapping_matrix(transformation_matrix_buffer);
    log_matrix_4x4(transformation_matrix_buffer[8]);
    map_world_space_vectors_to_screen_coordinates(
    randomised_triangle_vectors, screen_vectors, 
    &transformation_matrix_buffer[FINAL_MAPPING_MATRIX], triangle_number,
    WIDTH, HEIGHT);

    printf("new vectors\n"); 
    log_triangle_vectors_2d(screen_vectors, triangle_number);

    const float MOVE_SPEED  = 0.1f;
    const float ANGLE_SPEED = 5.0f * (M_PI / 180.0f);  // degrees per frame (or rad, your choice
  
    // running = false; 
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
          randomised_triangle_vectors, screen_vectors, 
          &transformation_matrix_buffer[FINAL_MAPPING_MATRIX], triangle_number,
        WIDTH, HEIGHT);

        // For debugging
        printf("pos: (%f, %f, %f) | yaw: %f pitch: %f roll: %f\n",
            main_camera.position.x, main_camera.position.y, main_camera.position.z, 
            main_camera.yaw , main_camera.pitch, main_camera.roll);

        // Fill the buffer with black 
        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                pixels[y * WIDTH + x] = (Uint32) (255 << 24) | (0 << 16) | (0 << 8) | 0;
            }
        }
        

        // Recolour any pixels that pass rasterising check to random colour
        rasterise_screen_coordinates(screen_vectors, triangle_number, randomised_colours, pixels, WIDTH);

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
