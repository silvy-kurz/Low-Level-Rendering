#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdbool.h>
#include "matrix_math.h"



int main(int argc, char *argv[]) {

  printf("this file is used to test out linear algebra / math implementations\n"); 
  struct screen_vector test_vector_a = {-200,0};
  struct screen_vector test_vector_b = {0.0,0.0};
  struct screen_vector test_vector_c = {0.0,5.0};
  log_vector(add_vector(test_vector_a, test_vector_b)); 
  // test for addition
  log_vector(divide_vector(test_vector_b, 4.3)); 
  log_vector(multiply_vector(test_vector_b, 8.6));
  float dot = dot_product(test_vector_b, test_vector_c);
  float len = length_vector(test_vector_b);
  printf("%f\n", dot);
  printf("%f\n", len);
  log_vector(normalise(perpendicularise(test_vector_a)));
  struct screen_vector random_vector = {random_integer(0, 10), random_integer(0,10)};
  log_vector(random_vector);
  

  int triangle_num = 1;
  struct world_vector* triangle_vectors = create_random_triangle_vectors(0, 99.9, 0, 99.9, 0, 99.9, triangle_num);
  int* triangle_colours = create_random_triangle_colours(triangle_num);
  
  log_triangle_vectors(triangle_vectors, triangle_colours, triangle_num);
  struct matrix testing_matrix = create_randomised_matrix(initalise_null_matrix(4,4), 0, 10);
  struct matrix testing_matrix_a = initalise_null_matrix(2,2);
  struct matrix testing_matrix_b = initalise_null_matrix(2,2);
  testing_matrix_a.data[0][0] = 3;
  testing_matrix_a.data[0][1] = 5;
  testing_matrix_a.data[1][0] = 1;
  testing_matrix_a.data[1][1] = 2;
  testing_matrix_b.data[0][0] = 2;
  testing_matrix_b.data[0][1] = 4;
  testing_matrix_b.data[1][0] = 1;
  testing_matrix_b.data[1][1] = 2;
  log_matrix(testing_matrix);
  log_matrix(testing_matrix_a);
  log_matrix(testing_matrix_b);
  log_matrix(multiply_matrix(testing_matrix_a, testing_matrix_b));

  printf("Testing Adding a Cube:\n");
  triangle_num = 0;
  struct world_vector* cube_triangle_vectors = create_random_triangle_vectors(0, 99.9, 0, 99.9, 0, 99.9, triangle_num);
  triangle_colours = create_random_triangle_colours(triangle_num + 2);
  struct world_vector starting_point = {0,0,0};
  cube_triangle_vectors = add_cube_to_world_vectors(cube_triangle_vectors, triangle_num, starting_point, 20);
  log_triangle_vectors(cube_triangle_vectors, triangle_colours, triangle_num + 2);
  struct screen_vector testing_point = {500, 500};
  struct screen_vector cube_vector_a = {cube_triangle_vectors[0].x, cube_triangle_vectors[0].y};
  struct screen_vector cube_vector_b = {cube_triangle_vectors[1].x, cube_triangle_vectors[1].y};
  struct screen_vector cube_vector_c = {cube_triangle_vectors[2].x, cube_triangle_vectors[2].y};
  bool triangle_works = is_point_in_triangle(cube_vector_a, cube_vector_b, cube_vector_c, testing_point);

  printf("%b\n", triangle_works);
  return 0;
}
