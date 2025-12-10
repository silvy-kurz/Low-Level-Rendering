#ifndef  VECTORS_AND_MATRICES_H
#define VECTORS_AND_MATRICES_H 

#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

//
// ======== DATA STRUCTURES ========
//

struct vector_2d {
  float x;
  float y;
};

struct vector_3d {
  float x;
  float y; 
  float z;
};

struct vector_4d {
  float x;
  float y;
  float z;
  float h;
};

struct matrix_3x3 {
  float data[9];
};

struct matrix_4x4 {
  float data[16];
};

struct matrix_n_m {
  int rows;
  int columns;
  float *data;
};

struct camera {
  struct vector_3d position;
  float yaw, pitch, roll;
  float near, far;
  float field_of_view;
  float aspect_ratio;
};

struct frustum_state {
  float x_scalar;
  float y_scalar;
  float z_scalar;
  float z_constant;
};

enum matrix_buffer_indexes {
  X_ROTATION_MATRIX,
  Y_ROTATION_MATRIX,
  X_Y_ROTATION_MATRIX,
  Z_ROTATION_MATRIX,
  X_Y_Z_ROTATION_MATRIX,
  POSITION_TRANSLATION_MATRIX,
  CAMERA_VIEW_MATRIX,
  PROJECTION_MATRIX,
  FINAL_MAPPING_MATRIX 
};

//
// ======== VECTOR ARITHMETIC ========
//

struct vector_2d add_vector_2d(struct vector_2d vector_a, struct vector_2d vector_b);
struct vector_2d subtract_vector_2d(struct vector_2d vector_a, struct vector_2d vector_b);
struct vector_2d multiply_vector_2d_scalar(struct vector_2d vector, float scalar);
struct vector_2d divide_vector_2d_scalar(struct vector_2d vector, float scalar);
float cross_product_vector_2d(struct vector_2d vector_a, struct vector_2d vector_b);

float length_vector_2d(struct vector_2d vector);
struct vector_2d normalise_vector_2d(struct vector_2d vector);
struct vector_2d perpendicularise_vector_2d(struct vector_2d vector);
float dot_product_vector_2d(struct vector_2d vector_a, struct vector_2d vector_b);


struct vector_3d add_vector_3d(struct vector_3d vector_a, struct vector_3d vector_b);
struct vector_3d subtract_vector_3d(struct vector_3d vector_a, struct vector_3d vector_b);
struct vector_3d multiply_vector_3d_scalar(struct vector_3d vector_a, float scalar);
struct vector_3d divide_vector_3d_scalar(struct vector_3d vector_a, float scalar);
struct vector_3d cross_product_vector_3d(struct vector_3d vector_a, struct vector_3d vector_b);

float length_vector_3d(struct vector_3d vector);
struct vector_3d normalise_vector_3d(struct vector_3d vector);
struct vector_3d perpendicularise_vector_3d(struct vector_3d vector);
float dot_product_vector_3d(struct vector_3d vector_a, struct vector_3d vector_b);

struct vector_2d copy_vector_3d_to_2d(struct vector_3d);
struct vector_3d copy_vector_4d_to_3d(struct vector_4d);
struct vector_4d convert_vector_3d_homogenous_coordinate(struct vector_3d vector);
struct vector_3d convert_homogenous_coordinate_vector_3d(struct vector_4d vector);
struct vector_2d map_norm_device_coordinate_to_screen(struct vector_3d norm_device_coordinate, int screen_width, int screen_height);

//
// ======== MATRIX ARITHMETIC ========
//

float get_element_matrix_3x3(struct matrix_3x3 *matrix_address, int row_index, int coloumn_index);
void set_element_matrix_3x3(struct matrix_3x3 *matrix_address,  int row_index, int coloumn_index, float value);
void initialise_null_matrix_3x3(struct matrix_3x3 *matrix_address);
void initialise_identity_matrix_3x3(struct matrix_3x3 *matrix_address);

struct vector_3d multiply_matrix_3x3_v3(struct matrix_3x3 *matrix_address, struct vector_3d vector);
void multiply_matrix_3x3(struct matrix_3x3 *matrix_a_address, struct matrix_3x3 *matrix_b_address, struct matrix_3x3 *product_matrix_address);


float get_element_matrix_4x4(struct matrix_4x4 *matrix_address, int row_index, int coloumn_index);
void set_element_matrix_4x4(struct matrix_4x4 *matrix_address, int row_index, int coloumn_index, float value);
void initialise_null_matrix_4x4(struct matrix_4x4 *matrix_address);
void initialise_identity_matrix_4x4(struct matrix_4x4 *matrix_address);

struct vector_4d multiply_matrix_4x4_v4(struct matrix_4x4 *matrix_address, struct vector_4d vector);
void multiply_matrix_4x4(struct matrix_4x4 *matrix_a_address, struct matrix_4x4 *matrix_b_address, struct matrix_4x4 *product_matrix_address);


float get_element_matrix_n_m(struct matrix_n_m *matrix_address, int row_index, int coloumn_index);
void set_element_matrix_n_m(struct matrix_n_m *matrix_address, int row_index, int coloumn_index, float value);
void initialise_null_matrix_n_m(struct matrix_n_m *matrix_address);
void initialise_identity_matrix_n_m(struct matrix_n_m *matrix_address);

void multiply_matrix_n_m(struct matrix_n_m *matrix_a_address, struct matrix_n_m *matrix_b_address, struct matrix_n_m *product_matrix_address);

//
// ======== CAMERAS ========
//

struct frustum_state calculate_frustum_values(struct camera camera);

//
// ======== TRANSLATIONS ========
//

void update_x_rotation_matrix(struct matrix_4x4 *matrix_address, float angle_radians);
void update_y_rotation_matrix(struct matrix_4x4 *matrix_address, float angle_radians);
void update_z_rotation_matrix(struct matrix_4x4 *matrix_address, float angle_radians);

void update_translation_matrix(struct matrix_4x4 *matrix_address, struct vector_3d translation_position);
void update_scaling_matrix(struct matrix_4x4 *matrix_address, float scalar);
void update_projection_matrix(struct matrix_4x4 *matrix_address, struct frustum_state frustum_data);
void calculate_mapping_matrix(struct matrix_4x4 *matrices_buffer);

void initialise_matrix_buffer(struct matrix_4x4 *matrices_buffer, int buffer_size, int *matrix_initial_states);

void map_world_space_vectors_to_screen_coordinates(
  struct vector_3d *world_space_vectors, struct vector_2d *screen_coordinates, 
  struct matrix_4x4 *mapping_matrix_address, int triangle_number,
  int screen_width, int screen_height);

//
// ======== TRIANGLE OPERATIONS ========
//

bool is_point_right_side_line(struct vector_2d vector_a, struct vector_2d vector_b, struct vector_2d test_vector);
bool is_point_in_triangle(struct vector_2d vector_a, struct vector_2d vector_b, struct vector_2d vector_c, struct vector_2d test_vector);
void rasterise_screen_coordinates(struct vector_2d *screen_coordinates, int triangle_number, Uint32 *triangle_colours, Uint32 *pixel_buffer, int screen_width);

//
// ======== LOGGING ========
//

void log_vector_2d(struct vector_2d vector);
void log_vector_3d(struct vector_3d vector);
void log_vector_4d(struct vector_4d vector);

void log_matrix_3x3(struct matrix_3x3 matrix);
void log_matrix_4x4(struct matrix_4x4 matrix);
void log_matrix_n_m(struct matrix_n_m matrix);

void log_camera(struct camera camera);
void log_colour(int colour);
void log_triangle_vectors_2d(struct vector_2d *triangle_vectors, int triangle_number);
void log_triangle_vectors_3d(struct vector_3d *triangle_vectors, int triangle_number);
void log_triangle_colours(int *colours, int triangle_number);

//
// ======== SCENE FILLING ========
//

int get_random_integer(int low, int high);
float get_random_float(float low, float high);

void fill_random_vectors_3d(struct vector_3d *triangle_vectors_address, int triangle_number,
                            int min_x, int max_x,
                            int min_y, int max_y,
                            int min_z, int max_z
                            ); 
void fill_random_colours(int *colours_address, int triangle_number);

void fill_cube_vectors_3d(struct vector_3d *triangle_vectors_address, 
                          struct vector_3d bottom_left_corner, float cube_size);
#endif
