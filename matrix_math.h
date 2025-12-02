#ifndef MATRIX_MATH_H
#define MATRIX_MATH_H 

#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

// ---------- Structs ----------
struct world_vector {
    float x;
    float y;
    float z;
};

struct screen_vector {
    float x;
    float y;
};

// ---------- Random ----------
int random_integer(int low, int high);
float random_float(float low, float high);

// ---------- Vector operations ----------
struct screen_vector add_vector(struct screen_vector a, struct screen_vector b);
struct screen_vector subtract_vector(struct screen_vector a, struct screen_vector b);
struct screen_vector multiply_vector(struct screen_vector v, float scalar);
struct screen_vector divide_vector(struct screen_vector v, float scalar);
float dot_product(struct screen_vector a, struct screen_vector b);
float length_vector(struct screen_vector v);
struct screen_vector perpendicularise(struct screen_vector v);
struct screen_vector normalise(struct screen_vector v);

// ---------- Logging ----------
void log_vector(struct screen_vector vector);
void log_world_vector(struct world_vector vector);
void log_colour(int colour);

// ---------- Random geometry ----------
struct world_vector* create_random_triangle_vectors(
    float min_x, float max_x,
    float min_y, float max_y,
    float min_z, float max_z,
    int number_of_triangles
);

int* create_random_triangle_colours(int number_of_triangles);

// ---------- Triangle operations ----------
bool is_point_in_triangle(
    struct screen_vector a,
    struct screen_vector b,
    struct screen_vector c,
    struct screen_vector p
);

// ---------- Projection ----------
struct screen_vector world_space_to_screen_space_map(
    struct world_vector world,
    struct screen_vector screen_pixels,
    float fov,
    float smaller_dimension
);

#endif
