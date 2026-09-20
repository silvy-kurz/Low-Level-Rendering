#include <SDL2/SDL.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
//
// ======== SCENE FILLING ========
//

int
get_random_integer (int low, int high)
{
  return low + arc4random_uniform (high - low + 1);
}

float
get_random_float (float low, float high)
{
  float r = (float)arc4random () / (float)UINT32_MAX;

  return low + r * (high - low);
}

void
fill_random_vectors_3d (vector_3d *triangle_vectors_address, int triangle_number, int min_x, int max_x, int min_y,
                        int max_y, int min_z, int max_z)
{
  for (int vector_num = 0; vector_num < triangle_number * 3; vector_num++)
    {
      triangle_vectors_address[vector_num] = (vector_3d){
        get_random_float (min_x, max_x),
        get_random_float (min_y, max_y),
        get_random_float (min_z, max_z),
      };
    }
}

void
fill_random_colours (Uint32 *colours_address, int triangle_number)
{
  for (int colour_num = 0; colour_num < triangle_number; colour_num++)
    {
      Uint8 r = get_random_integer (0, 255);
      Uint8 g = get_random_integer (0, 255);
      Uint8 b = get_random_integer (0, 255);
      int rand_colour = 0 << 24 | r << 16 | g << 8 | b;
      colours_address[colour_num] = rand_colour;
    }
}

void
fill_cube_vectors_3d (vector_3d *triangle_vectors_address, vector_3d bottom_left_corner, float cube_size)
{
  vector_3d cube_corners[8];
  cube_corners[0] = (vector_3d){ bottom_left_corner.x + 0, bottom_left_corner.y + cube_size, bottom_left_corner.z + 0 };

  cube_corners[1]
      = (vector_3d){ bottom_left_corner.x + cube_size, bottom_left_corner.y + cube_size, bottom_left_corner.z + 0 };

  cube_corners[2] = (vector_3d){ bottom_left_corner.x + cube_size, bottom_left_corner.y + 0, bottom_left_corner.z + 0 };

  cube_corners[3] = (vector_3d){ bottom_left_corner.x + 0, bottom_left_corner.y + 0, bottom_left_corner.z + 0 };

  cube_corners[4] = (vector_3d){ bottom_left_corner.x + 0, bottom_left_corner.y + 0, bottom_left_corner.z + cube_size };

  cube_corners[5]
      = (vector_3d){ bottom_left_corner.x + cube_size, bottom_left_corner.y + 0, bottom_left_corner.z + cube_size };

  cube_corners[6] = (vector_3d){ bottom_left_corner.x + cube_size, bottom_left_corner.y + cube_size,
                                 bottom_left_corner.z + cube_size };

  cube_corners[7]
      = (vector_3d){ bottom_left_corner.x + 0, bottom_left_corner.y + cube_size, bottom_left_corner.z + cube_size };

  // first triangle
  triangle_vectors_address[0] = cube_corners[0];
  triangle_vectors_address[1] = cube_corners[1];
  triangle_vectors_address[2] = cube_corners[2];

  // second triangle
  triangle_vectors_address[3] = cube_corners[0];
  triangle_vectors_address[4] = cube_corners[2];
  triangle_vectors_address[5] = cube_corners[3];

  // third triangle
  triangle_vectors_address[6] = cube_corners[7];
  triangle_vectors_address[7] = cube_corners[0];
  triangle_vectors_address[8] = cube_corners[3];
  // fourth triangle
  triangle_vectors_address[9] = cube_corners[7];
  triangle_vectors_address[10] = cube_corners[3];
  triangle_vectors_address[11] = cube_corners[4];

  // FIFTH TRIANGLE (Right Face)
  triangle_vectors_address[12] = cube_corners[1];
  triangle_vectors_address[13] = cube_corners[6];
  triangle_vectors_address[14] = cube_corners[5];
  // SIXTH TRIANGLE
  triangle_vectors_address[15] = cube_corners[1];
  triangle_vectors_address[16] = cube_corners[5];
  triangle_vectors_address[17] = cube_corners[2];

  // SEVENTH TRIANGLE (Back Face)
  triangle_vectors_address[18] = cube_corners[6];
  triangle_vectors_address[19] = cube_corners[7];
  triangle_vectors_address[20] = cube_corners[4];
  // EIGHTH TRIANGLE
  triangle_vectors_address[21] = cube_corners[6];
  triangle_vectors_address[22] = cube_corners[4];
  triangle_vectors_address[23] = cube_corners[5];

  // NINTH TRIANGLE (Top Face)
  triangle_vectors_address[24] = cube_corners[7];
  triangle_vectors_address[25] = cube_corners[6];
  triangle_vectors_address[26] = cube_corners[1];
  // TENTH TRIANGLE
  triangle_vectors_address[27] = cube_corners[7];
  triangle_vectors_address[28] = cube_corners[1];
  triangle_vectors_address[29] = cube_corners[0];

  // ELEVENTH TRIANGLE (Bottom Face)
  triangle_vectors_address[30] = cube_corners[3];
  triangle_vectors_address[31] = cube_corners[2];
  triangle_vectors_address[32] = cube_corners[5];
  // TWELFTH TRIANGLE
  triangle_vectors_address[33] = cube_corners[3];
  triangle_vectors_address[34] = cube_corners[5];
  triangle_vectors_address[35] = cube_corners[4];
}

// Writes exactly 20 triangles (60 vertices).
void
fill_floor_vectors_3d (vector_3d *triangles, vector_3d centre, float width, float depth)
{
  const int columns = 5;
  const int rows = 2;

  float tile_width = width / columns;
  float tile_depth = depth / rows;

  float start_x = centre.x - width / 2.0f;
  float start_z = centre.z - depth / 2.0f;

  int next = 0;

  for (int row = 0; row < rows; row++)
    {
      for (int column = 0; column < columns; column++)
        {
          float x0 = start_x + column * tile_width;
          float x1 = start_x + (column + 1) * tile_width;
          float z0 = start_z + row * tile_depth;
          float z1 = start_z + (row + 1) * tile_depth;

          vector_3d a = { x0, centre.y, z0 };
          vector_3d b = { x0, centre.y, z1 };
          vector_3d c = { x1, centre.y, z1 };
          vector_3d d = { x1, centre.y, z0 };

          // Winding gives both triangles an upward (+Y) normal.
          triangles[next++] = a;
          triangles[next++] = b;
          triangles[next++] = c;

          triangles[next++] = a;
          triangles[next++] = c;
          triangles[next++] = d;
        }
    }
}
