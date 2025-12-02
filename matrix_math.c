#include "matrix_math.h"

#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

int random_integer(int low, int high) {
    return low + arc4random_uniform(high - low + 1);
}

float random_float(float low, float high) {
  float r = (float)arc4random() / (float)UINT32_MAX;

    return low + r * (high - low);
}

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

struct screen_vector multiply_vector(const struct screen_vector vector, float scalar) { 
    struct screen_vector product_vector;
    product_vector.x = vector.x * scalar; 
    product_vector.y = vector.y * scalar; 

    return product_vector; 
}

struct screen_vector divide_vector(const struct screen_vector vector, const float scalar) {
    struct screen_vector qoutient_vector;
    qoutient_vector.x = vector.x / scalar; 
    qoutient_vector.y = vector.y / scalar; 

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
  printf("Vector: (%f, %f)\n", vector.x, vector.y);
}

void log_world_vector(const struct world_vector vector) {
  printf("Vector: (%f, %f, %f)\n", vector.x, vector.y, vector.z);
}
void log_colour(int colour) {
  uint8_t a = (colour >> 24) & 0xFF;
  uint8_t r = (colour >> 16) & 0xFF;
  uint8_t g = (colour >> 8) & 0xFF;
  uint8_t b = (colour >> 0) & 0xFF;
  printf("Colour %d: (%u,%u,%u)\n", colour, r, g, b, a);
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

int* create_random_triangle_colours(int number_of_triangles) {
  int* random_colours = malloc(sizeof(int) * number_of_triangles);
  for (int colour_num = 0; colour_num < number_of_triangles; colour_num++) {
      Uint8 r = random_integer(0, 255); 
      Uint8 g = random_integer(0, 255); 
      Uint8 b = random_integer(0, 255); 
      int rand_colour = 0 << 24 | r << 16 | g << 8 | b;
      random_colours[colour_num] = rand_colour;
  }

  return random_colours;
}

bool is_point_right_side_line(const struct screen_vector vector_a, const struct screen_vector vector_b, const struct screen_vector screen_point) {
  float point_to_a_area = dot_product(
    subtract_vector(screen_point, vector_a),
    perpendicularise(subtract_vector(vector_b, vector_a))
  ) / 2;

  return point_to_a_area >= 0;
}

bool is_point_in_triangle(const struct screen_vector vector_a, const struct screen_vector vector_b, 
                          const struct screen_vector vector_c, const struct screen_vector screen_point) {

    bool sign_p_to_a = is_point_right_side_line(vector_a, vector_b, screen_point);
    bool sign_p_to_b = is_point_right_side_line(vector_b, vector_c, screen_point);
    bool sign_p_to_c = 

    return sign_p_to_a && sign_p_to_b && sign_p_to_c;
}

struct screen_vector world_space_to_screen_space_map(const struct world_vector vector_in_world, 
                                                     const struct screen_vector pixels_number_vector,
                                                     float FOV, float smaller_dimension) {
    float total_units_visible = tan(FOV / 2) * 2;    
    float pixel_scaling_factor = smaller_dimension / total_units_visible;
    
    struct screen_vector offset_vector = {vector_in_world.x, vector_in_world.y};
    struct screen_vector pixel_offset =  multiply_vector(offset_vector, pixel_scaling_factor);
    struct screen_vector final_converted_screen_space_vector = add_vector(pixel_offset, divide_vector(pixels_number_vector, 2));

    return final_converted_screen_space_vector; 
}

