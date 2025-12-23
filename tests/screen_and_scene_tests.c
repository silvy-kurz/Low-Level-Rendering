#include "../src/screen_and_scene.c"
#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>



int arena_tests()
{
  size_t testing_capacity = sizeof(vector_3d) * 8;
  memory_arena testing_arena = {
    .capacity = testing_capacity,
    .memory = malloc(testing_capacity),
    .current_address_offset = 0,
  };
  
  printf("%d memory\n", testing_arena.memory);
  printf("%d offset\n", testing_arena.current_address_offset);
  vector_3d *vec_list = allocate_arena_space(sizeof(vector_3d) * 4, &testing_arena);
  printf("%d given memory\n", vec_list);
  printf("%d offset\n", testing_arena.current_address_offset);
  printf("%d memory\n", testing_arena.memory);
  vector_2d *screen_vec_list = allocate_arena_space(sizeof(vector_4d) * 2, &testing_arena);
  printf("%d given memory\n", screen_vec_list);
  printf("%d offset\n", testing_arena.current_address_offset);
  printf("%d memory\n", testing_arena.memory);

  return 0;
}


