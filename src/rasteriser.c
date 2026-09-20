#include <SDL2/SDL.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

//
// ======== TRIANGLE OPERATIONS ========
//

bool
is_point_right_side_line (vector_2d vector_a, vector_2d vector_b, vector_2d test_vector)
{
  float cross_product
      = cross_product_vector_2d (subtract_vector_2d (vector_b, vector_a), subtract_vector_2d (test_vector, vector_a));

  return cross_product <= 0;
}

bool
is_point_in_triangle (vector_2d vector_a, vector_2d vector_b, vector_2d vector_c, vector_2d test_vector)
{
  bool sign_p_to_a = is_point_right_side_line (vector_a, vector_b, test_vector);
  bool sign_p_to_b = is_point_right_side_line (vector_b, vector_c, test_vector);
  bool sign_p_to_c = is_point_right_side_line (vector_c, vector_a, test_vector);

  return sign_p_to_a && sign_p_to_b && sign_p_to_c;
}

void
rasterise_screen_coordinates (vector_2d *screen_coordinates, int triangle_number, Uint32 *triangle_colours,
                              Uint32 *pixel_buffer, int screen_width, int screen_height)
{
  for (int triangle_index = 0; triangle_index < triangle_number; triangle_index++)
    {
      vector_2d triangle_vector_a = screen_coordinates[triangle_index * 3 + 0];
      vector_2d triangle_vector_b = screen_coordinates[triangle_index * 3 + 1];
      vector_2d triangle_vector_c = screen_coordinates[triangle_index * 3 + 2];

      if (!isfinite (triangle_vector_a.x) || !isfinite (triangle_vector_a.y) || !isfinite (triangle_vector_b.x)
          || !isfinite (triangle_vector_b.y) || !isfinite (triangle_vector_c.x) || !isfinite (triangle_vector_c.y))
        {
          continue;
        }

      float left = fminf (triangle_vector_a.x, fminf (triangle_vector_b.x, triangle_vector_c.x));
      float right = fmaxf (triangle_vector_a.x, fmaxf (triangle_vector_b.x, triangle_vector_c.x));
      float top = fminf (triangle_vector_a.y, fminf (triangle_vector_b.y, triangle_vector_c.y));
      float bottom = fmaxf (triangle_vector_a.y, fmaxf (triangle_vector_b.y, triangle_vector_c.y));

      int min_x = (int)floorf (fmaxf (0.0f, fminf (left, screen_width)));
      int max_x = (int)ceilf (fmaxf (0.0f, fminf (right, screen_width)));
      int min_y = (int)floorf (fmaxf (0.0f, fminf (top, screen_height)));
      int max_y = (int)ceilf (fmaxf (0.0f, fminf (bottom, screen_height)));

      for (int y = min_y; y < max_y; y++)
        {
          for (int x = min_x; x < max_x; x++)
            {
              vector_2d current_pixel = { x, y };

              if (is_point_in_triangle (triangle_vector_a, triangle_vector_b, triangle_vector_c, current_pixel))
                {
                  pixel_buffer[y * screen_width + x] = triangle_colours[triangle_index];
                }
            }
        }
    }
}
