#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>

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

void log_vector(const struct screen_vector vector) {
  printf("Vector (%f, %f)\n", vector.x, vector.y);
}
int main(int argc, char *argv[]) {

  printf("this file will contain all the matrix math implementations\n");
  struct screen_vector test_vector_a = {0,0};
  struct screen_vector test_vector_b = {5.5,3.2};
  struct screen_vector test_vector_c = {4.1,8.9};
  log_vector(add_vector(test_vector_a, test_vector_b)); 
  log_vector(add_vector(test_vector_a, test_vector_b)); 
  log_vector(divide_vector(test_vector_b, test_vector_c)); 
  log_vector(multiply_vector(test_vector_b, test_vector_c));
  float dot = dot_product(test_vector_b, test_vector_c);
  float len = length_vector(test_vector_b);
  printf("%f\n", dot);
  printf("%f\n", len);
  log_vector(normalise(perpendicularise(test_vector_b)));
  return 0;
}
