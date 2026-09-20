#include <SDL2/SDL.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

//
// ======== LOGGING ========
//

void
log_vector_2d (vector_2d vector)
{
  printf ("2D Vector: (x: %f, y: %f)\n", vector.x, vector.y);
}
void
log_vector_3d (vector_3d vector)
{
  printf ("3D Vector: (x: %f, y: %f, z: %f)\n", vector.x, vector.y, vector.z);
}

void
log_vector_4d (vector_4d vector)
{
  printf ("4D Vector: (x: %f, y: %f, z: %f, h: %f)\n", vector.x, vector.y, vector.z, vector.h);
}

void
log_matrix_3x3 (matrix_3x3 matrix)
{
  float *data = matrix.data;
  printf ("3 x 3 Matrix: {\n");
  printf ("    {%f %f %f}\n", data[0], data[1], data[2]);
  printf ("    {%f %f %f}\n", data[3], data[4], data[5]);
  printf ("    {%f %f %f}}\n", data[6], data[7], data[8]);
}

void
log_matrix_4x4 (matrix_4x4 matrix)
{
  float *data = matrix.data;
  printf ("4 x 4 Matrix: {\n");
  printf ("    {%f %f %f %f}\n", data[0], data[1], data[2], data[3]);
  printf ("    {%f %f %f %f}\n", data[4], data[5], data[6], data[7]);
  printf ("    {%f %f %f %f}\n", data[8], data[9], data[10], data[11]);
  printf ("    {%f %f %f %f}}\n", data[12], data[13], data[14], data[15]);
}

void
log_matrix_n_m (matrix_n_m matrix)
{
  printf ("%d x %d Matrix: \n", matrix.rows, matrix.columns);
  printf ("{\n");
  for (int row_index = 0; row_index < matrix.rows; row_index++)
    {
      printf ("  {");
      for (int column_index = 0; column_index < matrix.columns; column_index++)
        {
          printf ("%f ", matrix.data[row_index * matrix.columns + column_index]);
        }
      printf ("}\n");
    }
  printf ("}\n");
}

void
log_camera (camera camera)
{
  printf ("Camera: \n");
  printf ("  Position: ");
  log_vector_3d (camera.position);
  printf ("  Orientation: (yaw: %f, pitch: %f, roll: %f)\n", camera.yaw, camera.pitch, camera.roll);
  printf ("  Planes: (near: %f, far: %f)\n", camera.near, camera.far);
  printf ("  Field of View: %f\n", camera.field_of_view);
  printf ("  Aspect Ratio: %f\n", camera.aspect_ratio);
  printf ("  \n");
}

void
log_colour (int colour)
{
  uint8_t r = (colour >> 16) & 0xFF;
  uint8_t g = (colour >> 8) & 0xFF;
  uint8_t b = (colour >> 0) & 0xFF;
  printf ("Colour %d: (%u,%u,%u)\n", colour, r, g, b);
}

void
log_triangle_vectors_2d (vector_2d *triangle_vectors, int triangle_number)
{
  for (int triangle_index = 0; triangle_index < triangle_number; triangle_index++)
    {
      printf ("Triangle %d:\n", triangle_index);
      printf ("  ");
      log_vector_2d (triangle_vectors[triangle_index * 3]);
      printf ("\n");
      printf ("  ");
      log_vector_2d (triangle_vectors[triangle_index * 3 + 1]);
      printf ("\n");
      printf ("  ");
      log_vector_2d (triangle_vectors[triangle_index * 3 + 2]);
      printf ("\n");
      printf ("  \n");
    }
}

void
log_triangle_vectors_3d (vector_3d *triangle_vectors, int triangle_number)
{
  for (int vector_index = 0; vector_index < triangle_number * 3; vector_index++)
    {
      log_vector_3d (triangle_vectors[vector_index]);
    }
}

void
log_triangle_colours (Uint32 *colours, int triangle_number)
{
  for (int colour_index = 0; colour_index < triangle_number; colour_index++)
    {
      log_colour (colours[colour_index]);
    }
}
