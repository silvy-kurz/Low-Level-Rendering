#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

//
// ======== DATA STRUCTURES ========
//

typedef struct {
  float x;
  float y;
} vector_2d;

typedef struct {
  float x;
  float y; 
  float z;
} vector_3d;

typedef struct {
  float x;
  float y;
  float z;
  float h;
} vector_4d;

typedef struct {
  float data[9];
} matrix_3x3;

typedef struct {
  float data[16];
} matrix_4x4;

typedef struct {
  int rows;
  int columns;
  float *data;
} matrix_n_m;

typedef struct {
  vector_3d position;
  float yaw, pitch, roll;
  float near, far;
  float field_of_view;
  float aspect_ratio;
} camera;

typedef struct {
  float x_scalar;
  float y_scalar;
  float z_scalar;
  float z_constant;
} frustum_state;

enum matrix_buffer_indexes {
  X_ROTATION_MATRIX,
  Y_ROTATION_MATRIX,
  X_Y_ROTATION_MATRIX,
  Z_ROTATION_MATRIX,
  X_Y_Z_ROTATION_MATRIX,
  POSITION_TRANSLATION_MATRIX,
  CAMERA_VIEW_MATRIX,
  PROJECTION_MATRIX,
  FINAL_MAPPING_MATRIX,
  TOTAL_MATRIX_COUNT
};

//
// ======== VECTOR ARITHMETIC ========
//

vector_2d add_vector_2d(vector_2d vector_a, vector_2d vector_b) {
  return (vector_2d) {vector_a.x + vector_b.x, vector_a.y + vector_b.y};
}

vector_2d subtract_vector_2d(vector_2d vector_a, vector_2d vector_b) {
  return (vector_2d) {vector_a.x - vector_b.x, vector_a.y - vector_b.y};
}

vector_2d multiply_vector_2d_scalar(vector_2d vector, float scalar) {
  return (vector_2d) {vector.x * scalar, vector.y * scalar};
}

vector_2d divide_vector_2d_scalar(vector_2d vector, float scalar) {
  return (vector_2d) {vector.x / scalar, vector.y / scalar};
}

float cross_product_vector_2d(vector_2d vector_a, vector_2d vector_b) {
  return vector_a.x * vector_b.y - vector_b.x * vector_a.y; 
}

float length_vector_2d(vector_2d vector) {
  return sqrtf(vector.x * vector.x + vector.y * vector.y);
}

vector_2d normalise_vector_2d(vector_2d vector) {
  float vector_length = length_vector_2d(vector);
  return (vector_2d) {vector.x / vector_length, vector.y / vector_length};
}

vector_2d perpendicularise_vector_2d(vector_2d vector) {
  return (vector_2d) {vector.y, -vector.x};
}

float dot_product_vector_2d(vector_2d vector_a, vector_2d vector_b) {
  return vector_a.x * vector_b.x + vector_a.y * vector_b.y;
}


vector_3d add_vector_3d(vector_3d vector_a, vector_3d vector_b) {
  return (vector_3d) {vector_a.x + vector_b.x, vector_a.y + vector_b.y, vector_a.z + vector_b.z};
}

vector_3d subtract_vector_3d(vector_3d vector_a, vector_3d vector_b) {
  return (vector_3d) {vector_a.x - vector_b.x, vector_a.y - vector_b.y, vector_a.z - vector_b.z};
}

vector_3d multiply_vector_3d_scalar(vector_3d vector, float scalar) {
  return (vector_3d) {vector.x * scalar, vector.y * scalar, vector.z * scalar};
}

vector_3d divide_vector_3d_scalar(vector_3d vector, float scalar) {
  return (vector_3d) {vector.x / scalar, vector.y / scalar, vector.z / scalar};
}

vector_3d cross_product_vector_3d(vector_3d vector_a, vector_3d vector_b) {
  printf("(3D CROSS PRODUCT LOGIC NOT IMPLEMENTED YET)");
  return (vector_3d) {0,0,0}; //TODO: Add in cross product implementation 
}

float length_vector_3d(vector_3d vector) {
  return sqrtf(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
}

vector_3d normalise_vector_3d(vector_3d vector) {
  float vector_length = length_vector_3d(vector);
  return (vector_3d) {vector.x / vector_length, vector.y / vector_length, vector.z / vector_length};
}

vector_3d perpendicularise_vector_3d(vector_3d vector) {
  printf("(3D CROSS PRODUCT LOGIC NOT IMPLEMENTED YET)");
  return (vector_3d) {0,0,0}; //TODO: Add in 3d vector perpendicular implementation
}
float dot_product_vector_3d(vector_3d vector_a, vector_3d vector_b) {
  return vector_a.x + vector_b.x + vector_a.y + vector_b.y + vector_a.z + vector_b.z;
}

vector_2d copy_vector_3d_to_2d(vector_3d vector) {
  return (vector_2d) {vector.x, vector.y};
}

vector_3d copy_vector_4d_to_3d(vector_4d vector) {
  return (vector_3d) {vector.x, vector.y, vector.z};
}
vector_4d convert_vector_3d_homogenous_coordinate(vector_3d vector) {
  return (vector_4d) {vector.x, vector.y, vector.z, 1};
}

vector_3d convert_homogenous_coordinate_vector_3d(vector_4d vector) {
  return (vector_3d) {vector.x / vector.h + 0.5, vector.y / vector.h + 0.5, vector.z / vector.h + 0.5};
}

vector_2d map_norm_device_coordinate_to_screen(vector_3d norm_device_coordinate, int screen_width, int screen_height) {
  return (vector_2d) {norm_device_coordinate.x * screen_width, (1 - norm_device_coordinate.y) * screen_height};
}

//
// ======== MATRIX ARITHMETIC ========
//

float get_element_matrix_3x3(matrix_3x3 *matrix_address, int row_index, int column_index) {
  return matrix_address->data[3 * row_index + column_index];
}
void set_element_matrix_3x3(matrix_3x3 *matrix_address, int row_index, int column_index, float value) {
  matrix_address->data[3 * row_index + column_index] = value;
}

void initialise_null_matrix_3x3(matrix_3x3 *matrix_address) {
  set_element_matrix_3x3(matrix_address, 0, 0, 0);
  set_element_matrix_3x3(matrix_address, 0, 1, 0);
  set_element_matrix_3x3(matrix_address, 0, 2, 0);

  set_element_matrix_3x3(matrix_address, 1, 0, 0);
  set_element_matrix_3x3(matrix_address, 1, 1, 0);
  set_element_matrix_3x3(matrix_address, 1, 2, 0);

  set_element_matrix_3x3(matrix_address, 2, 0, 0);
  set_element_matrix_3x3(matrix_address, 2, 1, 0);
  set_element_matrix_3x3(matrix_address, 2, 2, 0);
}

void initialise_identity_matrix_3x3(matrix_3x3 *matrix_address) {
  set_element_matrix_3x3(matrix_address, 0,0,1);
  set_element_matrix_3x3(matrix_address, 0,1,0);
  set_element_matrix_3x3(matrix_address, 0,2,0);
  
  set_element_matrix_3x3(matrix_address, 1,0,0);
  set_element_matrix_3x3(matrix_address, 1,1,1);
  set_element_matrix_3x3(matrix_address, 1,2,0);

  set_element_matrix_3x3(matrix_address, 2,0,0);
  set_element_matrix_3x3(matrix_address, 2,1,0);
  set_element_matrix_3x3(matrix_address, 2,2,1);
}

vector_3d multiply_matrix_3x3_v3(matrix_3x3 *matrix_address, vector_3d vector) {
  float *data = matrix_address->data;
  return (vector_3d) {
    data[0] * vector.x +
    data[1] * vector.y +
    data[2] * vector.z,

    data[3] * vector.x +
    data[4] * vector.y +
    data[5] * vector.z,

    data[6] * vector.x +
    data[7] * vector.y +
    data[8] * vector.z,
  };
}

void multiply_matrix_3x3(matrix_3x3 *matrix_a_address, matrix_3x3 *matrix_b_address, matrix_3x3 *product_matrix_address) {
  float *data_a = matrix_a_address->data;
  float *data_b = matrix_b_address->data;
  float *out_data = product_matrix_address->data;

  out_data[0] = data_a[0] * data_b[0] + data_a[1] * data_b[3] + data_a[2] * data_b[6];
  out_data[1] = data_a[0] * data_b[1] + data_a[1] * data_b[4] + data_a[2] * data_b[7];
  out_data[2] = data_a[0] * data_b[2] + data_a[1] * data_b[5] + data_a[2] * data_b[8];

  out_data[3] = data_a[3] * data_b[0] + data_a[4] * data_b[3] + data_a[5] * data_b[6];
  out_data[4] = data_a[3] * data_b[1] + data_a[4] * data_b[4] + data_a[5] * data_b[7];
  out_data[5] = data_a[3] * data_b[2] + data_a[4] * data_b[5] + data_a[5] * data_b[8];

  out_data[6] = data_a[6] * data_b[0] + data_a[7] * data_b[3] + data_a[8] * data_b[6];
  out_data[7] = data_a[6] * data_b[1] + data_a[7] * data_b[4] + data_a[8] * data_b[7];
  out_data[8] = data_a[6] * data_b[2] + data_a[7] * data_b[5] + data_a[8] * data_b[8];
}


float get_element_matrix_4x4(matrix_4x4 *matrix_address, int row_index, int column_index) { 
  return matrix_address->data[4 * row_index + column_index];
}

void set_element_matrix_4x4(matrix_4x4 *matrix_address, int row_index, int column_index, float value) {
  matrix_address->data[4 * row_index + column_index] = value;
}

void initialise_null_matrix_4x4(matrix_4x4 *matrix_address) {
  set_element_matrix_4x4(matrix_address, 0, 0, 0);
  set_element_matrix_4x4(matrix_address, 0, 1, 0);
  set_element_matrix_4x4(matrix_address, 0, 2, 0);
  set_element_matrix_4x4(matrix_address, 0, 3, 0);

  set_element_matrix_4x4(matrix_address, 1, 0, 0);
  set_element_matrix_4x4(matrix_address, 1, 1, 0);
  set_element_matrix_4x4(matrix_address, 1, 2, 0);
  set_element_matrix_4x4(matrix_address, 1, 3, 0);

  set_element_matrix_4x4(matrix_address, 2, 0, 0);
  set_element_matrix_4x4(matrix_address, 2, 1, 0);
  set_element_matrix_4x4(matrix_address, 2, 2, 0);
  set_element_matrix_4x4(matrix_address, 2, 3, 0);

  set_element_matrix_4x4(matrix_address, 3, 0, 0);
  set_element_matrix_4x4(matrix_address, 3, 1, 0);
  set_element_matrix_4x4(matrix_address, 3, 2, 0);
  set_element_matrix_4x4(matrix_address, 3, 3, 0);
}

void initialise_identity_matrix_4x4(matrix_4x4 *matrix_address) {
  set_element_matrix_4x4(matrix_address, 0, 0, 1);
  set_element_matrix_4x4(matrix_address, 0, 1, 0);
  set_element_matrix_4x4(matrix_address, 0, 2, 0);
  set_element_matrix_4x4(matrix_address, 0, 3, 0);

  set_element_matrix_4x4(matrix_address, 1, 0, 0);
  set_element_matrix_4x4(matrix_address, 1, 1, 1);
  set_element_matrix_4x4(matrix_address, 1, 2, 0);
  set_element_matrix_4x4(matrix_address, 1, 3, 0);

  set_element_matrix_4x4(matrix_address, 2, 0, 0);
  set_element_matrix_4x4(matrix_address, 2, 1, 0);
  set_element_matrix_4x4(matrix_address, 2, 2, 1);
  set_element_matrix_4x4(matrix_address, 2, 3, 0);

  set_element_matrix_4x4(matrix_address, 3, 0, 0);
  set_element_matrix_4x4(matrix_address, 3, 1, 0);
  set_element_matrix_4x4(matrix_address, 3, 2, 0);
  set_element_matrix_4x4(matrix_address, 3, 3, 1);
}

vector_4d multiply_matrix_4x4_v4(matrix_4x4 *matrix_address, vector_4d vector) {
  float *data = matrix_address->data;
  return (vector_4d) {
    data[0] * vector.x +
    data[1] * vector.y +
    data[2] * vector.z +
    data[3] * vector.h,

    data[4] * vector.x +
    data[5] * vector.y +
    data[6] * vector.z +
    data[7] * vector.h,

    data[8] * vector.x +
    data[9] * vector.y +
    data[10] * vector.z +
    data[11] * vector.h,

    data[12] * vector.x +
    data[13] * vector.y +
    data[14] * vector.z +
    data[15] * vector.h,
  };
}

void multiply_matrix_4x4(matrix_4x4 *matrix_a_address, matrix_4x4 *matrix_b_address, matrix_4x4 *product_matrix_address) {
  float *data_a = matrix_a_address->data;
  float *data_b = matrix_b_address->data;
  float *out_data = product_matrix_address->data;

  out_data[0] = data_a[0] * data_b[0] + data_a[1] * data_b[4] + data_a[2] * data_b[8] + data_a[3] * data_b[12];
  out_data[1] = data_a[0] * data_b[1] + data_a[1] * data_b[5] + data_a[2] * data_b[9] + data_a[3] * data_b[13];
  out_data[2] = data_a[0] * data_b[2] + data_a[1] * data_b[6] + data_a[2] * data_b[10] + data_a[3] * data_b[14];
  out_data[3] = data_a[0] * data_b[3] + data_a[1] * data_b[7] + data_a[2] * data_b[11] + data_a[3] * data_b[15];

  out_data[4] = data_a[4] * data_b[0] + data_a[5] * data_b[4] + data_a[6] * data_b[8] + data_a[7] * data_b[12];
  out_data[5] = data_a[4] * data_b[1] + data_a[5] * data_b[5] + data_a[6] * data_b[9] + data_a[7] * data_b[13];
  out_data[6] = data_a[4] * data_b[2] + data_a[5] * data_b[6] + data_a[6] * data_b[10] + data_a[7] * data_b[14];
  out_data[7] = data_a[4] * data_b[3] + data_a[5] * data_b[7] + data_a[6] * data_b[11] + data_a[7] * data_b[15];

  out_data[8] = data_a[8] * data_b[0] + data_a[9] * data_b[4] + data_a[10] * data_b[8] + data_a[11] * data_b[12];
  out_data[9] = data_a[8] * data_b[1] + data_a[9] * data_b[5] + data_a[10] * data_b[9] + data_a[11] * data_b[13];
  out_data[10] = data_a[8] * data_b[2] + data_a[9] * data_b[6] + data_a[10] * data_b[10] + data_a[11] * data_b[14];
  out_data[11] = data_a[8] * data_b[3] + data_a[9] * data_b[7] + data_a[10] * data_b[11] + data_a[11] * data_b[15];

  out_data[12] = data_a[12] * data_b[0] + data_a[13] * data_b[4] + data_a[14] * data_b[8] + data_a[15] * data_b[12];
  out_data[13] = data_a[12] * data_b[1] + data_a[13] * data_b[5] + data_a[14] * data_b[9] + data_a[15] * data_b[13];
  out_data[14] = data_a[12] * data_b[2] + data_a[13] * data_b[6] + data_a[14] * data_b[10] + data_a[15] * data_b[14];
  out_data[15] = data_a[12] * data_b[3] + data_a[13] * data_b[7] + data_a[14] * data_b[11] + data_a[15] * data_b[15];
}


float get_element_matrix_n_m(matrix_n_m *matrix_address, int row_index, int column_index) {
  return matrix_address->data[matrix_address->columns * row_index + column_index];
}

void set_element_matrix_n_m(matrix_n_m *matrix_address, int row_index, int column_index, float value) {
  matrix_address->data[matrix_address->rows * row_index + column_index] = value;
}

void initialise_null_matrix_n_m(matrix_n_m *matrix_address) {
  for (int row_index = 0; row_index < matrix_address->rows; row_index++) {
    for (int column_index = 0; column_index < matrix_address->columns; column_index++) {
      set_element_matrix_n_m(matrix_address, row_index, column_index, 0);
    }
  }
}

void initialise_identity_matrix_n_m(matrix_n_m *matrix_address) {
  for (int row_index = 0; row_index < matrix_address->rows; row_index++) {
    for (int column_index = 0; column_index < matrix_address->columns; column_index++) {
      set_element_matrix_n_m(matrix_address, row_index, column_index, 0);
    }
  }


  for (int row_index = 0; row_index < matrix_address->rows; row_index++) {
    set_element_matrix_n_m(matrix_address, row_index, row_index, 1);
  }
}

void multiply_matrix_n_m(matrix_n_m *matrix_a_address, matrix_n_m *matrix_b_address, matrix_n_m *product_matrix_address) {
  initialise_null_matrix_n_m(product_matrix_address); //TODO: Implement general matrix multiplication
}

//
// ======== CAMERAS ========
//

frustum_state calculate_frustum_values(camera camera) {
  float near = camera.near;
  float far = camera.far;
  return (frustum_state) {
    camera.aspect_ratio * 1 / (2 * tanf(camera.field_of_view / 2)),
    1 / (2 * tanf(camera.field_of_view / 2)),
    (far + near) / (2 * (near - far)),
    (near * far) / (near - far)
  };
}

//
// ======== TRANSLATIONS ========
//

void update_x_rotation_matrix(matrix_4x4 *matrix_address, float angle_radians) {
  float *data = matrix_address->data;             // equivalent to matrix {
  sincosf(angle_radians, &data[9], &data[5]);  //                   {1      0       0       0} 
  data[6] = -data[9];                          //                   {0      cos(a)  -sin(a) 0} 
  data[10] = data[5];                          //                   {0      sin(a)  cos(a)  0}  
}                                              //                   {0      0       0      1}}

void update_y_rotation_matrix(matrix_4x4 *matrix_address, float angle_radians) {
  float *data = matrix_address->data;             // equivalent to matrix {
  sincosf(angle_radians, &data[2], &data[0]);  //                   {cos(a)  0       sin(a)  0} 
  data[8] = -data[2];                          //                   {0       1       0       0} 
  data[10] = data[0];                          //                   {-sin(a) 0       cos(a)  0}  
}                                              //                   {0       0       0      1}}

void update_z_rotation_matrix(matrix_4x4 *matrix_address, float angle_radians) {
  float *data = matrix_address->data;             // equivalent to matrix {
  sincosf(angle_radians, &data[4], &data[0]);  //                   {cos(a) -sin(a) 0       0} 
  data[1] = -data[4];                          //                   {sin(a) cos(a)  0       0} 
  data[5] = data[0];                          //                   {0      0       1       0}  
}                                              //                   {0      0       0       1}}

void update_translation_matrix(matrix_4x4 *matrix_address, vector_3d translation_position) {
  float *data = matrix_address->data;
  data[3] = translation_position.x;
  data[7] = translation_position.y;
  data[11] = translation_position.z;
}

void update_scaling_matrix(matrix_4x4 *matrix_address, float scalar) {
  float *data = matrix_address->data;
  data[0] = scalar;
  data[5] = scalar;
  data[10] = scalar;
  data[15] = scalar;
}

void update_projection_matrix(matrix_4x4 *matrix_address, frustum_state frustum_data) {
  float *data = matrix_address->data;

  data[0] = frustum_data.x_scalar;
  data[5] = frustum_data.y_scalar;
  data[10] = frustum_data.z_scalar;
  data[11] = frustum_data.z_constant;
  data[14] = -1;
  data[15] = 0;
}

void initialise_matrix_buffer(matrix_4x4 *matrices_buffer, int *matrix_initial_states) {
  for (int matrix_index = 0; matrix_index < TOTAL_MATRIX_COUNT; matrix_index++) {
    switch (matrix_initial_states[matrix_index]) {
      case 0:
        initialise_null_matrix_4x4(&matrices_buffer[matrix_index]);
        break;

      case 1:
        initialise_identity_matrix_4x4(&matrices_buffer[matrix_index]);
        break;
    }
  }
}

void calculate_mapping_matrix(matrix_4x4 *matrices_buffer) { 
  multiply_matrix_4x4(&matrices_buffer[X_ROTATION_MATRIX], &matrices_buffer[Y_ROTATION_MATRIX], &matrices_buffer[X_Y_ROTATION_MATRIX]);
  multiply_matrix_4x4(&matrices_buffer[X_Y_ROTATION_MATRIX], &matrices_buffer[Z_ROTATION_MATRIX], &matrices_buffer[X_Y_Z_ROTATION_MATRIX]);
  multiply_matrix_4x4(&matrices_buffer[X_Y_Z_ROTATION_MATRIX], &matrices_buffer[POSITION_TRANSLATION_MATRIX], &matrices_buffer[CAMERA_VIEW_MATRIX]);
  multiply_matrix_4x4(&matrices_buffer[PROJECTION_MATRIX], &matrices_buffer[CAMERA_VIEW_MATRIX], &matrices_buffer[FINAL_MAPPING_MATRIX]);
}

void map_world_space_vectors_to_screen_coordinates(
  vector_3d *world_space_vectors, vector_2d *screen_coordinates, 
  matrix_4x4 *mapping_matrix_address, int triangle_number,
  int screen_width, int screen_height) {
  for (int i = 0; i < triangle_number * 3; i++) {
    vector_4d homogenous_coordinate = convert_vector_3d_homogenous_coordinate(world_space_vectors[i]);

    // log_vector_4d(homogenous_coordinate);
    vector_4d transformed_4d_vector = multiply_matrix_4x4_v4(mapping_matrix_address, homogenous_coordinate);
      
    // log_vector_4d(transformed_4d_vector);
    vector_3d normalised_device_coordinate = convert_homogenous_coordinate_vector_3d(transformed_4d_vector);
    

    vector_2d screen_coordinate = map_norm_device_coordinate_to_screen(normalised_device_coordinate, screen_width, screen_height);

    screen_coordinates[i] = screen_coordinate;

  }
}

//
// ======== TRIANGLE OPERATIONS ========
//

bool is_point_right_side_line(vector_2d vector_a, vector_2d vector_b, vector_2d test_vector) {
  float cross_product = cross_product_vector_2d(
    subtract_vector_2d(vector_b, vector_a),
    subtract_vector_2d(test_vector, vector_a)
  );

  return cross_product >= 0;
}

bool is_point_in_triangle(vector_2d vector_a, vector_2d vector_b, vector_2d vector_c, vector_2d test_vector) {
    bool sign_p_to_a = is_point_right_side_line(vector_a, vector_b, test_vector);
    bool sign_p_to_b = is_point_right_side_line(vector_b, vector_c, test_vector);
    bool sign_p_to_c = is_point_right_side_line(vector_c, vector_a, test_vector);

    return sign_p_to_a && sign_p_to_b && sign_p_to_c;
}

void rasterise_screen_coordinates(vector_2d *screen_coordinates,
                                  int triangle_number, Uint32 *triangle_colours, 
                                  Uint32 *pixel_buffer, int screen_width) {
  for (int triangle_index = 0; triangle_index < triangle_number; triangle_index++) {
    vector_2d triangle_vector_a = screen_coordinates[triangle_index * 3 + 0];
    vector_2d triangle_vector_b = screen_coordinates[triangle_index * 3 + 1];
    vector_2d triangle_vector_c = screen_coordinates[triangle_index * 3 + 2];
    
    int min_x = fmin(triangle_vector_a.x, fmin(triangle_vector_b.x, triangle_vector_c.x));
    int max_x = fmax(triangle_vector_a.x, fmin(triangle_vector_b.x, triangle_vector_b.x));
    int min_y = fmin(triangle_vector_a.y, fmin(triangle_vector_b.y, triangle_vector_c.y));
    int max_y = fmax(triangle_vector_a.y, fmax(triangle_vector_b.y, triangle_vector_c.y));
          
    for (int y = min_y; y < max_y; y++) {
        for (int x = min_x; x < max_x; x++) {
          vector_2d current_pixel = {x, y};
                
          if (is_point_in_triangle(
              triangle_vector_a, 
              triangle_vector_b, 
              triangle_vector_c, 
              current_pixel)) {
            pixel_buffer[y * screen_width + x] = triangle_colours[triangle_index]; 
          }
        }
      }
  }
}

//
// ======== LOGGING ========
//

void log_vector_2d(vector_2d vector) {
  printf("2D Vector: (x: %f, y: %f)\n", vector.x, vector.y);
}
void log_vector_3d(vector_3d vector) {
  printf("3D Vector: (x: %f, y: %f, z: %f)\n", vector.x, vector.y, vector.z);
}

void log_vector_4d(vector_4d vector) {
  printf("4D Vector: (x: %f, y: %f, z: %f, h: %f)\n", vector.x, vector.y, vector.z, vector.h);
}

void log_matrix_3x3(matrix_3x3 matrix) {
  float *data = matrix.data;
  printf("3 x 3 Matrix: {\n");
  printf("    {%f %f %f}\n", data[0], data[1], data[2]);
  printf("    {%f %f %f}\n", data[3], data[4], data[5]);
  printf("    {%f %f %f}}\n", data[6], data[7], data[8]);
}

void log_matrix_4x4(matrix_4x4 matrix) {
  float *data = matrix.data;
  printf("4 x 4 Matrix: {\n");
  printf("    {%f %f %f %f}\n", data[0], data[1], data[2], data[3]);
  printf("    {%f %f %f %f}\n", data[4], data[5], data[6], data[7]);
  printf("    {%f %f %f %f}\n", data[8], data[9], data[10], data[11]);
  printf("    {%f %f %f %f}}\n", data[12], data[13], data[14], data[15]);
}

void log_matrix_n_m(matrix_n_m matrix) {
  printf("%d x %d Matrix: \n", matrix.rows, matrix.columns);
  printf("{\n");
  for (int row_index = 0; row_index < matrix.rows; row_index++) {
    printf("  {");
    for (int column_index = 0; column_index < matrix.columns; column_index++) {
      printf("%f ", matrix.data[row_index * matrix.columns + column_index]);
    }
    printf("}\n");
  }
  printf("}\n");

}

void log_camera(camera camera) {
  printf("Camera: \n");
  printf("  Position: ");
  log_vector_3d(camera.position);
  printf("  Orientation: (yaw: %f, pitch: %f, roll: %f)\n", camera.yaw, camera.pitch, camera.roll);
  printf("  Planes: (near: %f, far: %f)\n", camera.near, camera.far);
  printf("  Field of View: %f\n", camera.field_of_view); 
  printf("  Aspect Ratio: %f\n", camera.aspect_ratio); 
  printf("  \n");
}

void log_colour(int colour) {
  uint8_t r = (colour >> 16) & 0xFF;
  uint8_t g = (colour >> 8) & 0xFF;
  uint8_t b = (colour >> 0) & 0xFF;
  printf("Colour %d: (%u,%u,%u)\n", colour, r, g, b);
}

void log_triangle_vectors_2d(vector_2d *triangle_vectors, int triangle_number) {
  for (int vector_index = 0; vector_index < triangle_number * 3; vector_index++) {
    log_vector_2d(triangle_vectors[vector_index]);
  }
}

void log_triangle_vectors_3d(vector_3d *triangle_vectors, int triangle_number) {
  for (int vector_index = 0; vector_index < triangle_number * 3; vector_index++) {
    log_vector_3d(triangle_vectors[vector_index]);
  }
}

void log_triangle_colours(int *colours, int triangle_number) {
  for (int colour_index = 0; colour_index < triangle_number; colour_index++) {
    log_colour(colours[colour_index]);
  }
}

//
// ======== SCENE FILLING ========
//

int get_random_integer(int low, int high) {
    return low + arc4random_uniform(high - low + 1);
}

float get_random_float(float low, float high) {
  float r = (float)arc4random() / (float)UINT32_MAX;

    return low + r * (high - low);
}

void fill_random_vectors_3d(vector_3d *triangle_vectors_address, int triangle_number,
                            int min_x, int max_x,
                            int min_y, int max_y,
                            int min_z, int max_z
                            ) {
  for (int vector_num = 0; vector_num < triangle_number * 3; vector_num ++) {
    triangle_vectors_address[vector_num] = (vector_3d){
      get_random_float(min_x, max_x),
      get_random_float(min_y, max_y),
      get_random_float(min_z, max_z),
    };
  }
}

void fill_random_colours(int *colours_address, int triangle_number) {
  for (int colour_num = 0; colour_num < triangle_number; colour_num++) {
      Uint8 r = get_random_integer(0, 255); 
      Uint8 g = get_random_integer(0, 255); 
      Uint8 b = get_random_integer(0, 255); 
      int rand_colour = 0 << 24 | r << 16 | g << 8 | b;
      colours_address[colour_num] = rand_colour;
  }
}

void fill_cube_vectors_3d(vector_3d *triangle_vectors_address, 
                          vector_3d bottom_left_corner, float cube_size) {
  vector_3d cube_corners[8];
  cube_corners[0] = (vector_3d) {
    bottom_left_corner.x + 0, 
    bottom_left_corner.y + cube_size, 
    bottom_left_corner.z + 0};

  cube_corners[1] = (vector_3d) {
    bottom_left_corner.x + cube_size, 
    bottom_left_corner.y + cube_size, 
    bottom_left_corner.z + 0};

  cube_corners[2] = (vector_3d) {
    bottom_left_corner.x + cube_size, 
    bottom_left_corner.y + 0, 
    bottom_left_corner.z + 0};

  cube_corners[3] = (vector_3d) {
    bottom_left_corner.x + 0, 
    bottom_left_corner.y + 0, 
    bottom_left_corner.z + 0};

  cube_corners[4] = (vector_3d) {
    bottom_left_corner.x + 0, 
    bottom_left_corner.y + 0, 
    bottom_left_corner.z + cube_size};

  cube_corners[5] = (vector_3d) {
    bottom_left_corner.x + cube_size, 
    bottom_left_corner.y + 0, 
    bottom_left_corner.z + cube_size};

  cube_corners[6] = (vector_3d) {
    bottom_left_corner.x + cube_size, 
    bottom_left_corner.y + cube_size, 
    bottom_left_corner.z + cube_size};

  cube_corners[7] = (vector_3d) {
    bottom_left_corner.x + 0, 
    bottom_left_corner.y + cube_size, 
    bottom_left_corner.z + cube_size};

  // first triangle
  triangle_vectors_address[0] = cube_corners[0];
  triangle_vectors_address[1] = cube_corners[1];
  triangle_vectors_address[2] = cube_corners[2];

  // second triangle
  triangle_vectors_address[3] = cube_corners[0];
  triangle_vectors_address[4] = cube_corners[2];
  triangle_vectors_address[5] = cube_corners[3];
}

