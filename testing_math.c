#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdbool.h>
#include "matrix_math.h"



int main(int argc, char *argv[]) {

  printf("this file will contain all the matrix math implementations\n");
  struct screen_vector test_vector_a = {-200,0};
  struct screen_vector test_vector_b = {0.0,0.0};
  struct screen_vector test_vector_c = {0.0,5.0};
  log_vector(add_vector(test_vector_a, test_vector_b)); 
  // test for addition
  log_vector(add_vector(test_vector_a, test_vector_b)); 
  log_vector(divide_vector(test_vector_b, 4.3)); 
  log_vector(multiply_vector(test_vector_b, 8.6));
  float dot = dot_product(test_vector_b, test_vector_c);
  float len = length_vector(test_vector_b);
  printf("%f\n", dot);
  printf("%f\n", len);
  log_vector(normalise(perpendicularise(test_vector_b)));
  struct screen_vector random_vector = {random_integer(0, 10), random_integer(0,10)};
  log_vector(random_vector);
  

  int triangle_num = 1;
  struct world_vector* triangle_vectors = create_random_triangle_vectors(0, 99.9, 0, 99.9, 0, 99.9, triangle_num);
  int* triangle_colours = create_random_triangle_colours(triangle_num);
  for (int i = 0; i < triangle_num; i++) {
    printf("First triangle vector:\n");
    log_world_vector(triangle_vectors[i * 3 + 0]);
    printf("  \n");
    printf("Second triangle vector:\n");
    log_world_vector(triangle_vectors[i * 3 + 1]);
    printf("  \n");
    printf("Third triangle vector:\n");
    log_world_vector(triangle_vectors[i * 3 + 2]);
    printf("  \n");
    printf("Triangle colour\n");
    log_colour(triangle_colours[i]);
    printf("=====\n");
  }
  printf("%b \n", is_point_right_side_line(test_vector_b, test_vector_c, test_vector_a));
  return 0;
}
