#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

//
// ======== ARENAS  ========
//

typedef struct {
  size_t capacity;
  void *memory;
  size_t current_address_offset;
} memory_arena;

void *allocate_arena_space(size_t data_size, memory_arena *arena) {
  void *requested_address;

  if (arena->current_address_offset + data_size > arena->capacity) {
    printf("Arena is Full or too much data is being requested!\n");
  } else {
    void *requested_address = arena->memory + arena->current_address_offset;
    arena->current_address_offset += data_size;
    return requested_address;
  }
}
