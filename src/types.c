#include <SDL2/SDL.h>
#include <inttypes.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef int32_t i32;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;

//
// ======== DATA STRUCTURES ========
//

typedef struct
{
  float x;
  float y;
} vector_2d;

typedef struct
{
  float x;
  float y;
  float z;
} vector_3d;

typedef struct
{
  float x;
  float y;
  float z;
  float h;
} vector_4d;

typedef struct
{
  float data[9];
} matrix_3x3;

typedef struct
{
  float data[16];
} matrix_4x4;

typedef struct
{
  int rows;
  int columns;
  float *data;
} matrix_n_m;

typedef struct
{
  vector_3d position;
  float yaw, pitch, roll;
  float near, far;
  float field_of_view;
  float aspect_ratio;
} camera;

typedef struct
{
  float x_scalar;
  float y_scalar;
  float z_scalar;
  float z_constant;
} frustum_state;

enum matrix_buffer_indexes
{
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
