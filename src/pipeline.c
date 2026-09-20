#include <SDL2/SDL.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void
map_world_space_vectors_to_screen_coordinates (vector_3d *world_space_vectors, vector_2d *screen_coordinates,
                                               matrix_4x4 *mapping_matrix_address, int triangle_number,
                                               int screen_width, int screen_height)
{
  for (int t = 0; t < triangle_number; t++)
    {
      vector_4d clip[3];
      bool reject = false;

      for (int j = 0; j < 3; j++)
        {
          clip[j] = multiply_matrix_4x4_v4 (mapping_matrix_address,
                                            convert_vector_3d_homogenous_coordinate (world_space_vectors[t * 3 + j]));

          // For your projection, h = -z in camera space.
          // Temporary near cutoff: 1 world unit.
          if (!isfinite (clip[j].h) || clip[j].h < 1.0f)
            {
              reject = true;
            }
        }

      for (int j = 0; j < 3; j++)
        {
          if (reject)
            {
              // The rasteriser's isfinite check skips this triangle.
              screen_coordinates[t * 3 + j] = (vector_2d){ NAN, NAN };
            }
          else
            {
              screen_coordinates[t * 3 + j] = map_norm_device_coordinate_to_screen (
                  convert_homogenous_coordinate_vector_3d (clip[j]), screen_width, screen_height);
            }
        }
    }
}
