#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

int random_integer(int low, int high) {
    return low + arc4random_uniform(high - low + 1);
}

float random_float(float low, float high) {
  float r = (float)arc4random() / (float)UINT32_MAX;

    return low + r * (high - low);
}

struct world_vector { // vectors in world space, pre screen map
  float x; 
  float y;
  float z;
};

struct screen_vector { // vector struct for screen space, post screen map 
  float x;
  float y;
};

struct screen_vector add_vector(const struct screen_vector vector_a, const struct screen_vector vector_b) {
    struct screen_vector sum_vector;
    sum_vector.x = vector_a.x + vector_b.x;
    sum_vector.y = vector_a.y + vector_b.y;

    return sum_vector; 
}

struct screen_vector subtract_vector(const struct screen_vector vector_a, const struct screen_vector vector_b) {
    struct screen_vector difference_vector;
    difference_vector.x = vector_a.x - vector_b.x;
    difference_vector.y = vector_a.y - vector_b.y;

    return difference_vector; 
}

struct screen_vector multiply_vector(const struct screen_vector vector_a, const struct screen_vector vector_b) {
    struct screen_vector product_vector;
    product_vector.x = vector_a.x * vector_b.x;
    product_vector.y = vector_a.y * vector_b.y;

    return product_vector; 
}

struct screen_vector divide_vector(const struct screen_vector vector_a, const struct screen_vector vector_b) {
    struct screen_vector qoutient_vector;
    qoutient_vector.x = vector_a.x / vector_b.x;
    qoutient_vector.y = vector_a.y / vector_b.y;

    return qoutient_vector; 
}

float dot_product(const struct screen_vector vector_a, const struct screen_vector vector_b) {

    return vector_a.x * vector_b.x + vector_a.y * vector_b.y; 
}

float length_vector(const struct screen_vector vector) {

  return sqrtf(vector.x * vector.x + vector.y * vector.y);
}

struct screen_vector perpendicularise(const struct screen_vector vector) {
  struct screen_vector perpendicular_vector;
  perpendicular_vector.x = vector.y; // this perpendicular option (y, -x) corresponds with counter-clockwise rotation 
  perpendicular_vector.y = -vector.x;// right-hand, counter-clockwise winding order 

  return perpendicular_vector;
}

struct screen_vector normalise(const struct screen_vector vector) {
  struct screen_vector normalised_vector;
  float vector_length = length_vector(vector);
  normalised_vector.x = vector.x / vector_length;
  normalised_vector.y = vector.y / vector_length;

  return normalised_vector;
}

 struct world_vector* create_random_triangle_vectors(float min_x, float max_x, 
                                                     float min_y, float max_y, 
                                                     float min_z, float max_z, int number_of_triangles) {
  struct world_vector* random_triangle_vectors = malloc(sizeof(struct world_vector) * number_of_triangles * 3);
  for (int vector_num = 0; vector_num < number_of_triangles * 3; vector_num ++) {
    struct world_vector random_vector = {
      random_float(min_x, max_x),
      random_float(min_y, max_y),
      random_float(min_z, max_z),
    };
    random_triangle_vectors[vector_num] = random_vector;
  }

  return random_triangle_vectors;
}

void log_vector(const struct screen_vector vector) {
  printf("Vector (%f, %f)\n", vector.x, vector.y);
}

void log_world_vector(const struct world_vector vector) {
  printf("Vector (%f, %f, %f)\n", vector.x, vector.y, vector.z);
}

int main(int argc, char *argv[]) {

  printf("this file will contain all the matrix math implementations\n");
  struct screen_vector test_vector_a = {0,0};
  struct screen_vector test_vector_b = {5.5,3.2};
  struct screen_vector test_vector_c = {4.1,8.9};
  log_vector(add_vector(test_vector_a, test_vector_b)); 
  // test for addition
  log_vector(add_vector(test_vector_a, test_vector_b)); 
  log_vector(divide_vector(test_vector_b, test_vector_c)); 
  log_vector(multiply_vector(test_vector_b, test_vector_c));
  float dot = dot_product(test_vector_b, test_vector_c);
  float len = length_vector(test_vector_b);
  printf("%f\n", dot);
  printf("%f\n", len);
  log_vector(normalise(perpendicularise(test_vector_b)));
  struct screen_vector random_vector = {random_integer(0, 10), random_integer(0,10)};
  log_vector(random_vector);
  

  int triangle_num = 2;
  struct world_vector* triangle_vectors = create_random_triangle_vectors(0, 99.9, 0, 99.9, 0, 99.9, triangle_num);
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
    printf("=====\n");
  }
  return 0;
}
