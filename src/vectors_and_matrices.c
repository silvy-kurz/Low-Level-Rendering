#include <SDL2/SDL.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

//
// ======== VECTOR ARITHMETIC ========
//

vector_2d
add_vector_2d (vector_2d vector_a, vector_2d vector_b)
{
  return (vector_2d){ vector_a.x + vector_b.x, vector_a.y + vector_b.y };
}

vector_2d
subtract_vector_2d (vector_2d vector_a, vector_2d vector_b)
{
  return (vector_2d){ vector_a.x - vector_b.x, vector_a.y - vector_b.y };
}

vector_2d
multiply_vector_2d_scalar (vector_2d vector, float scalar)
{
  return (vector_2d){ vector.x * scalar, vector.y * scalar };
}

vector_2d
divide_vector_2d_scalar (vector_2d vector, float scalar)
{
  return (vector_2d){ vector.x / scalar, vector.y / scalar };
}

float
cross_product_vector_2d (vector_2d vector_a, vector_2d vector_b)
{
  return vector_a.x * vector_b.y - vector_b.x * vector_a.y;
}

float
length_vector_2d (vector_2d vector)
{
  return sqrtf (vector.x * vector.x + vector.y * vector.y);
}

vector_2d
normalise_vector_2d (vector_2d vector)
{
  float vector_length = length_vector_2d (vector);
  return (vector_2d){ vector.x / vector_length, vector.y / vector_length };
}

vector_2d
perpendicularise_vector_2d (vector_2d vector)
{
  return (vector_2d){ vector.y, -vector.x };
}

float
dot_product_vector_2d (vector_2d vector_a, vector_2d vector_b)
{
  return vector_a.x * vector_b.x + vector_a.y * vector_b.y;
}

vector_3d
add_vector_3d (vector_3d vector_a, vector_3d vector_b)
{
  return (vector_3d){ vector_a.x + vector_b.x, vector_a.y + vector_b.y, vector_a.z + vector_b.z };
}

vector_3d
subtract_vector_3d (vector_3d vector_a, vector_3d vector_b)
{
  return (vector_3d){ vector_a.x - vector_b.x, vector_a.y - vector_b.y, vector_a.z - vector_b.z };
}

vector_3d
multiply_vector_3d_scalar (vector_3d vector, float scalar)
{
  return (vector_3d){ vector.x * scalar, vector.y * scalar, vector.z * scalar };
}

vector_3d
divide_vector_3d_scalar (vector_3d vector, float scalar)
{
  return (vector_3d){ vector.x / scalar, vector.y / scalar, vector.z / scalar };
}

vector_3d
cross_product_vector_3d (vector_3d vector_a, vector_3d vector_b)
{
  printf ("(3D CROSS PRODUCT LOGIC NOT IMPLEMENTED YET)");
  return (vector_3d){ vector_a.x * vector_b.x * 0, vector_a.y * vector_b.y * 0,
                      vector_a.z * vector_b.z * 0 }; // TODO: Add in cross product implementation
  //
}

float
length_vector_3d (vector_3d vector)
{
  return sqrtf (vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
}

vector_3d
normalise_vector_3d (vector_3d vector)
{
  float vector_length = length_vector_3d (vector);
  return (vector_3d){ vector.x / vector_length, vector.y / vector_length, vector.z / vector_length };
}

vector_3d
perpendicularise_vector_3d (vector_3d vector)
{
  printf ("(3D CROSS PRODUCT LOGIC NOT IMPLEMENTED YET)");
  return (vector_3d){ vector.x * 0, vector.y * 0, vector.z * 0 }; // TODO: Add in 3d vector perpendicular implementation
}
float
dot_product_vector_3d (vector_3d vector_a, vector_3d vector_b)
{
  return vector_a.x + vector_b.x + vector_a.y + vector_b.y + vector_a.z + vector_b.z;
}

vector_2d
copy_vector_3d_to_2d (vector_3d vector)
{
  return (vector_2d){ vector.x, vector.y };
}

vector_3d
copy_vector_4d_to_3d (vector_4d vector)
{
  return (vector_3d){ vector.x, vector.y, vector.z };
}
vector_4d
convert_vector_3d_homogenous_coordinate (vector_3d vector)
{
  return (vector_4d){ vector.x, vector.y, vector.z, 1 };
}

vector_3d
convert_homogenous_coordinate_vector_3d (vector_4d vector)
{
  return (vector_3d){ vector.x / vector.h + 0.5, vector.y / vector.h + 0.5, vector.z / vector.h + 0.5 };
}

vector_2d
map_norm_device_coordinate_to_screen (vector_3d norm_device_coordinate, int screen_width, int screen_height)
{
  return (vector_2d){ norm_device_coordinate.x * screen_width, (1 - norm_device_coordinate.y) * screen_height };
}

//
// ======== MATRIX ARITHMETIC ========
//

float
get_element_matrix_3x3 (matrix_3x3 *matrix_address, int row_index, int column_index)
{
  return matrix_address->data[3 * row_index + column_index];
}
void
set_element_matrix_3x3 (matrix_3x3 *matrix_address, int row_index, int column_index, float value)
{
  matrix_address->data[3 * row_index + column_index] = value;
}

void
initialise_null_matrix_3x3 (matrix_3x3 *matrix_address)
{
  set_element_matrix_3x3 (matrix_address, 0, 0, 0);
  set_element_matrix_3x3 (matrix_address, 0, 1, 0);
  set_element_matrix_3x3 (matrix_address, 0, 2, 0);

  set_element_matrix_3x3 (matrix_address, 1, 0, 0);
  set_element_matrix_3x3 (matrix_address, 1, 1, 0);
  set_element_matrix_3x3 (matrix_address, 1, 2, 0);

  set_element_matrix_3x3 (matrix_address, 2, 0, 0);
  set_element_matrix_3x3 (matrix_address, 2, 1, 0);
  set_element_matrix_3x3 (matrix_address, 2, 2, 0);
}

void
initialise_identity_matrix_3x3 (matrix_3x3 *matrix_address)
{
  set_element_matrix_3x3 (matrix_address, 0, 0, 1);
  set_element_matrix_3x3 (matrix_address, 0, 1, 0);
  set_element_matrix_3x3 (matrix_address, 0, 2, 0);

  set_element_matrix_3x3 (matrix_address, 1, 0, 0);
  set_element_matrix_3x3 (matrix_address, 1, 1, 1);
  set_element_matrix_3x3 (matrix_address, 1, 2, 0);

  set_element_matrix_3x3 (matrix_address, 2, 0, 0);
  set_element_matrix_3x3 (matrix_address, 2, 1, 0);
  set_element_matrix_3x3 (matrix_address, 2, 2, 1);
}

vector_3d
multiply_matrix_3x3_v3 (matrix_3x3 *matrix_address, vector_3d vector)
{
  float *data = matrix_address->data;
  return (vector_3d){
    data[0] * vector.x + data[1] * vector.y + data[2] * vector.z,

    data[3] * vector.x + data[4] * vector.y + data[5] * vector.z,

    data[6] * vector.x + data[7] * vector.y + data[8] * vector.z,
  };
}

void
multiply_matrix_3x3 (matrix_3x3 *matrix_a_address, matrix_3x3 *matrix_b_address, matrix_3x3 *product_matrix_address)
{
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

float
get_element_matrix_4x4 (matrix_4x4 *matrix_address, int row_index, int column_index)
{
  return matrix_address->data[4 * row_index + column_index];
}

void
set_element_matrix_4x4 (matrix_4x4 *matrix_address, int row_index, int column_index, float value)
{
  matrix_address->data[4 * row_index + column_index] = value;
}

void
initialise_null_matrix_4x4 (matrix_4x4 *matrix_address)
{
  set_element_matrix_4x4 (matrix_address, 0, 0, 0);
  set_element_matrix_4x4 (matrix_address, 0, 1, 0);
  set_element_matrix_4x4 (matrix_address, 0, 2, 0);
  set_element_matrix_4x4 (matrix_address, 0, 3, 0);

  set_element_matrix_4x4 (matrix_address, 1, 0, 0);
  set_element_matrix_4x4 (matrix_address, 1, 1, 0);
  set_element_matrix_4x4 (matrix_address, 1, 2, 0);
  set_element_matrix_4x4 (matrix_address, 1, 3, 0);

  set_element_matrix_4x4 (matrix_address, 2, 0, 0);
  set_element_matrix_4x4 (matrix_address, 2, 1, 0);
  set_element_matrix_4x4 (matrix_address, 2, 2, 0);
  set_element_matrix_4x4 (matrix_address, 2, 3, 0);

  set_element_matrix_4x4 (matrix_address, 3, 0, 0);
  set_element_matrix_4x4 (matrix_address, 3, 1, 0);
  set_element_matrix_4x4 (matrix_address, 3, 2, 0);
  set_element_matrix_4x4 (matrix_address, 3, 3, 0);
}

void
initialise_identity_matrix_4x4 (matrix_4x4 *matrix_address)
{
  set_element_matrix_4x4 (matrix_address, 0, 0, 1);
  set_element_matrix_4x4 (matrix_address, 0, 1, 0);
  set_element_matrix_4x4 (matrix_address, 0, 2, 0);
  set_element_matrix_4x4 (matrix_address, 0, 3, 0);

  set_element_matrix_4x4 (matrix_address, 1, 0, 0);
  set_element_matrix_4x4 (matrix_address, 1, 1, 1);
  set_element_matrix_4x4 (matrix_address, 1, 2, 0);
  set_element_matrix_4x4 (matrix_address, 1, 3, 0);

  set_element_matrix_4x4 (matrix_address, 2, 0, 0);
  set_element_matrix_4x4 (matrix_address, 2, 1, 0);
  set_element_matrix_4x4 (matrix_address, 2, 2, 1);
  set_element_matrix_4x4 (matrix_address, 2, 3, 0);

  set_element_matrix_4x4 (matrix_address, 3, 0, 0);
  set_element_matrix_4x4 (matrix_address, 3, 1, 0);
  set_element_matrix_4x4 (matrix_address, 3, 2, 0);
  set_element_matrix_4x4 (matrix_address, 3, 3, 1);
}

vector_4d
multiply_matrix_4x4_v4 (matrix_4x4 *matrix_address, vector_4d vector)
{
  float *data = matrix_address->data;
  return (vector_4d){
    data[0] * vector.x + data[1] * vector.y + data[2] * vector.z + data[3] * vector.h,

    data[4] * vector.x + data[5] * vector.y + data[6] * vector.z + data[7] * vector.h,

    data[8] * vector.x + data[9] * vector.y + data[10] * vector.z + data[11] * vector.h,

    data[12] * vector.x + data[13] * vector.y + data[14] * vector.z + data[15] * vector.h,
  };
}

void
multiply_matrix_4x4 (matrix_4x4 *matrix_a_address, matrix_4x4 *matrix_b_address, matrix_4x4 *product_matrix_address)
{
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

float
get_element_matrix_n_m (matrix_n_m *matrix_address, int row_index, int column_index)
{
  return matrix_address->data[matrix_address->columns * row_index + column_index];
}

void
set_element_matrix_n_m (matrix_n_m *matrix_address, int row_index, int column_index, float value)
{
  matrix_address->data[matrix_address->rows * row_index + column_index] = value;
}

void
initialise_null_matrix_n_m (matrix_n_m *matrix_address)
{
  for (int row_index = 0; row_index < matrix_address->rows; row_index++)
    {
      for (int column_index = 0; column_index < matrix_address->columns; column_index++)
        {
          set_element_matrix_n_m (matrix_address, row_index, column_index, 0);
        }
    }
}

void
initialise_identity_matrix_n_m (matrix_n_m *matrix_address)
{
  for (int row_index = 0; row_index < matrix_address->rows; row_index++)
    {
      for (int column_index = 0; column_index < matrix_address->columns; column_index++)
        {
          set_element_matrix_n_m (matrix_address, row_index, column_index, 0);
        }
    }

  for (int row_index = 0; row_index < matrix_address->rows; row_index++)
    {
      set_element_matrix_n_m (matrix_address, row_index, row_index, 1);
    }
}

void
multiply_matrix_n_m (matrix_n_m *matrix_a_address, matrix_n_m *matrix_b_address, matrix_n_m *product_matrix_address)
{
  initialise_null_matrix_n_m (product_matrix_address); // TODO: Implement general matrix multiplication
  set_element_matrix_n_m (product_matrix_address, 0, 0, matrix_a_address->data[0]);
  set_element_matrix_n_m (product_matrix_address, 1, 1, matrix_b_address->data[0]);
}

//
// ======== CAMERAS ========
//

frustum_state
calculate_frustum_values (camera camera)
{
  float near = camera.near;
  float far = camera.far;
  return (frustum_state){ camera.aspect_ratio * 1 / (2 * tanf (camera.field_of_view / 2)),
                          1 / (2 * tanf (camera.field_of_view / 2)), (far + near) / (2 * (near - far)),
                          (near * far) / (near - far) };
}

//
// ======== TRANSLATIONS ========
//

void
update_x_rotation_matrix (matrix_4x4 *matrix_address, float angle_radians)
{
  float *data = matrix_address->data; // equivalent to matrix {
  data[9] = sinf (angle_radians);
  data[5] = cosf (angle_radians);

  data[6] = -data[9]; //                   {0      cos(a)  -sin(a) 0}
  data[10] = data[5]; //                   {0      sin(a)  cos(a)  0}
} //                   {0      0       0      1}}

void
update_y_rotation_matrix (matrix_4x4 *matrix_address, float angle_radians)
{
  float *data = matrix_address->data; // equivalent to matrix {

  data[2] = sinf (angle_radians);
  data[0] = cosf (angle_radians);

  data[8] = -data[2]; //                   {0       1       0       0}
  data[10] = data[0]; //                   {-sin(a) 0       cos(a)  0}
} //                   {0       0       0      1}}

void
update_z_rotation_matrix (matrix_4x4 *matrix_address, float angle_radians)
{
  float *data = matrix_address->data; // equivalent to matrix {

  data[4] = sinf (angle_radians);
  data[0] = cosf (angle_radians);

  data[1] = -data[4]; //                   {sin(a) cos(a)  0       0}
  data[5] = data[0];  //                   {0      0       1       0}
} //                   {0      0       0       1}}

void
update_translation_matrix (matrix_4x4 *matrix_address, vector_3d translation_position)
{
  float *data = matrix_address->data;
  data[3] = translation_position.x;
  data[7] = translation_position.y;
  data[11] = translation_position.z;
}

void
update_scaling_matrix (matrix_4x4 *matrix_address, float scalar)
{
  float *data = matrix_address->data;
  data[0] = scalar;
  data[5] = scalar;
  data[10] = scalar;
  data[15] = scalar;
}

void
update_projection_matrix (matrix_4x4 *matrix_address, frustum_state frustum_data)
{
  float *data = matrix_address->data;

  data[0] = frustum_data.x_scalar;
  data[5] = frustum_data.y_scalar;
  data[10] = frustum_data.z_scalar;
  data[11] = frustum_data.z_constant;
  data[14] = -1;
  data[15] = 0;
}

void
initialise_matrix_buffer (matrix_4x4 *matrices_buffer, int *matrix_initial_states)
{
  for (int matrix_index = 0; matrix_index < TOTAL_MATRIX_COUNT; matrix_index++)
    {
      switch (matrix_initial_states[matrix_index])
        {
        case 0:
          initialise_null_matrix_4x4 (&matrices_buffer[matrix_index]);
          break;

        case 1:
          initialise_identity_matrix_4x4 (&matrices_buffer[matrix_index]);
          break;
        }
    }
}

void
calculate_mapping_matrix (matrix_4x4 *matrices_buffer)
{
  multiply_matrix_4x4 (&matrices_buffer[X_ROTATION_MATRIX], &matrices_buffer[Y_ROTATION_MATRIX],
                       &matrices_buffer[X_Y_ROTATION_MATRIX]);
  multiply_matrix_4x4 (&matrices_buffer[X_Y_ROTATION_MATRIX], &matrices_buffer[Z_ROTATION_MATRIX],
                       &matrices_buffer[X_Y_Z_ROTATION_MATRIX]);
  multiply_matrix_4x4 (&matrices_buffer[X_Y_Z_ROTATION_MATRIX], &matrices_buffer[POSITION_TRANSLATION_MATRIX],
                       &matrices_buffer[CAMERA_VIEW_MATRIX]);
  multiply_matrix_4x4 (&matrices_buffer[PROJECTION_MATRIX], &matrices_buffer[CAMERA_VIEW_MATRIX],
                       &matrices_buffer[FINAL_MAPPING_MATRIX]);
}
