#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

#include <pthread.h>

extern int WIDTH; 
extern int HEIGHT;


struct chunk {
  pthread_t thread_id;
  int *pixels;
  int chunk_number;
  bool *render_flag;
  bool *exit_flag;
  int size;
  int chunk_colour;
};

typedef struct chunk chunk_s;

void *render_chunk(void *arg) {
  chunk_s *chunk = (chunk_s*) arg;
  int thread_id = chunk->thread_id;
  int chunk_number = chunk->chunk_number;
  int *pixels = chunk->pixels;
  int size = chunk->size;
  bool *render_flag = chunk->render_flag;
  bool *exit_flag = chunk->exit_flag;
  int colour = chunk->chunk_colour;
  
  
  do {
    while (!*render_flag) {
      sleep(0);    // when not rendering sleep 
    }


//     rendering code 

//  printf("chunk number %d\n", chunk_number); 
//  printf("chunk starting point %d\n", (chunk_number * (size_x * size_y)));
//  printf("pixel buffer address %p for chunk %d\n", pixels, chunk_number);
  for (int i = 0; i < size; i++) {
      pixels[chunk_number * size + i] = colour;
    }

  }
  while (!*exit_flag);
  return 0;
}

int main(int argc, char *argv[]) {
  int POSITION_X;
  int POSITION_Y;
  int WIDTH = 1000;
  int HEIGHT = 1000;
  int borderless_flag;
  
    if (argc == 1) {
      WIDTH = 1000;
      HEIGHT = 999;
      borderless_flag = 0;
  } else if (argc == 2) {
    borderless_flag = 1;
  } else if (argc = 3) {
      WIDTH = atoi(argv[1]);
      HEIGHT = atoi(argv[2]);
      borderless_flag = 0;
  } {
  }
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("SDL init error: %s", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Window",
                                          POSITION_X,
                                          POSITION_Y, 
                                          WIDTH, HEIGHT,
                                          borderless_flag);
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
    
    // calculate chunk and thread paramaters
    int T = 10;
    int CHUNK_SIZE = floor((WIDTH * HEIGHT) / T);
    bool render_flag  = false; 
    bool exit_flag = false;
    bool MAIN_THREAD_EXIT_FLAG = true; // always true as we want main thread to only execute once 

    // initialise chunk and pixel buffers
    int *pixel_buffer = malloc(WIDTH * HEIGHT * sizeof(int));
    chunk_s *chunk_buffer = malloc(T * sizeof(chunk_s)); 
    
    // fill in chunk buffer 
    for (int i = 1; i < T; i ++) {
      chunk_buffer[i].chunk_number = i;

      chunk_buffer[i].size = CHUNK_SIZE;

      chunk_buffer[i].render_flag = &render_flag;
      chunk_buffer[i].exit_flag = &exit_flag;
      chunk_buffer[i].pixels = pixel_buffer;
      Uint8 r = rand() % 255;
      Uint8 g = rand() % 255;
      Uint8 b = rand() % 255;
      int rand_colour = 0 << 24 | r << 16 | g << 8 | b;

      chunk_buffer[i].chunk_colour = rand_colour;
      
  }
  chunk_buffer[0].chunk_number = 0;
  chunk_buffer[0].size = CHUNK_SIZE;

  chunk_buffer[0].render_flag = &render_flag;
  chunk_buffer[0].exit_flag = &MAIN_THREAD_EXIT_FLAG;
  chunk_buffer[0].pixels = pixel_buffer;
  chunk_buffer[0].chunk_colour = 0;
  

  
    // create threads 
    for (int i = 1; i < T; i++) {
    pthread_create(&chunk_buffer[i].thread_id, NULL, render_chunk, &chunk_buffer[i]);
  }    

    
    bool running = true;
    SDL_Event event;

    // Example pixel buffer

    while (running) {
        // Handle events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT){ 
                running = false;
                printf("holy quitting \n"); 
            }
            else if (event.type == SDL_KEYDOWN) {
                const Uint8 *state = SDL_GetKeyboardState(NULL);
                // Meta (Super) + Q to quit 
                if (state[SDL_SCANCODE_LGUI] && event.key.keysym.sym == SDLK_q)
                    running = false;
            }
        }

        // Fill the buffer with a gradient
        render_flag = 1;
        render_chunk(&chunk_buffer[0]);
        render_flag = 0;

        // Update texture with new pixels
        SDL_UpdateTexture(texture, NULL, pixel_buffer, WIDTH * sizeof(Uint32));
        
        // Render
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }
    
    
    free(chunk_buffer);
    free(pixel_buffer);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
