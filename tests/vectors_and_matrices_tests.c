#include "../src/vectors_and_matrices.c"
#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>



int all_tests()
{
  vector_2d testing_vector_2d_a = {7,2};
  vector_2d testing_vector_2d_b = {-1,2};
  vector_2d testing_vector_2d_c = {6,6};
  vector_2d testing_vector_2d_d = {0,0};

  printf("2D testing vectors:\n");
  printf("a = ");
  log_vector_2d(testing_vector_2d_a);
  printf("b = ");
  log_vector_2d(testing_vector_2d_b);
  printf("c = ");
  log_vector_2d(testing_vector_2d_c);
  printf("d = ");
  log_vector_2d(testing_vector_2d_d);
  printf(" \n");
  printf("Tests: \n");
  printf("b + c = ");
  log_vector_2d(add_vector_2d(testing_vector_2d_b, testing_vector_2d_c));
  printf("b - c = ");
  log_vector_2d(subtract_vector_2d(testing_vector_2d_b, testing_vector_2d_c));
  printf("c - b = ");
  log_vector_2d(subtract_vector_2d(testing_vector_2d_c, testing_vector_2d_b));
  printf("b * 2.5 = ");
  log_vector_2d(multiply_vector_2d_scalar(testing_vector_2d_b, 2.5));
  printf("c / 12 = ");
  log_vector_2d(divide_vector_2d_scalar(testing_vector_2d_c, 12));
  
  printf("(c+a) x b = %f\n", 
         cross_product_vector_2d(add_vector_2d(testing_vector_2d_c, testing_vector_2d_a), 
         testing_vector_2d_b));

  printf("|a| = %f\n", length_vector_2d(testing_vector_2d_a)); 
  printf("|c| = %f\n", length_vector_2d(testing_vector_2d_c)); 
  printf("norm(c) = ");
  log_vector_2d(normalise_vector_2d(testing_vector_2d_c));
  printf("perp(b) = ");
  log_vector_2d(perpendicularise_vector_2d(testing_vector_2d_b));
  printf("dot(a,b) = %f\n", 
  dot_product_vector_2d(testing_vector_2d_a, testing_vector_2d_b));



  vector_3d testing_vector_3d_a = {7,2, 3};
  vector_3d testing_vector_3d_b = {-1,2, 1};
  vector_3d testing_vector_3d_c = {6, 6, 6};
  vector_3d testing_vector_3d_d = {0, 0, 0};

  printf(" \n");
  printf(" \n");
  printf(" \n");
  printf("3D testing vectors:\n");
  printf("a = ");
  log_vector_3d(testing_vector_3d_a);
  printf("b = ");
  log_vector_3d(testing_vector_3d_b);
  printf("c = ");
  log_vector_3d(testing_vector_3d_c);
  printf("d = ");
  log_vector_3d(testing_vector_3d_d);
  printf(" \n");
  printf("Tests: \n");

  printf("b + c = ");
  log_vector_3d(add_vector_3d(testing_vector_3d_b, testing_vector_3d_c));

  printf("b - c = ");
  log_vector_3d(subtract_vector_3d(testing_vector_3d_b, testing_vector_3d_c));

  printf("c - b = ");
  log_vector_3d(subtract_vector_3d(testing_vector_3d_c, testing_vector_3d_b));

  printf("b * 2.5 = ");
  log_vector_3d(multiply_vector_3d_scalar(testing_vector_3d_b, 2.5));

  printf("c / 12 = ");
  log_vector_3d(divide_vector_3d_scalar(testing_vector_3d_c, 12));

  printf("c x b = ");
  log_vector_3d(
      cross_product_vector_3d(
          testing_vector_3d_c,
          testing_vector_3d_b
      )
  );

  printf("|a| = %f\n", length_vector_3d(testing_vector_3d_a));
  printf("|c| = %f\n", length_vector_3d(testing_vector_3d_c));

  printf("norm(c) = ");
  log_vector_3d(normalise_vector_3d(testing_vector_3d_c));

  printf("perp(b) = ");
  log_vector_3d(perpendicularise_vector_3d(testing_vector_3d_b));

  printf("dot(a,b) = %f\n",
         dot_product_vector_3d(testing_vector_3d_a,
                               testing_vector_3d_b));
    
  //
  // ======== 3x3 MATRIX TESTING ========
  //
  printf(" \n");
  printf(" \n");
  printf(" \n");
  matrix_3x3 A3;
  matrix_3x3 B3;
  matrix_3x3 P3;
  vector_3d v3 = { 1, 2, 3 };

  initialise_identity_matrix_3x3(&A3);
  initialise_null_matrix_3x3(&B3);

  printf("Matrix A3 (identity):\n");
  log_matrix_3x3(A3);

  printf("Matrix B3 (null):\n");
  log_matrix_3x3(B3);

  printf("\nSetting some elements in B3...\n");
  set_element_matrix_3x3(&B3, 0, 1, 5.5f);
  set_element_matrix_3x3(&B3, 2, 0, -3.0f);

  printf("B3 after changes:\n");
  log_matrix_3x3(B3);
  log_vector_3d(v3);
  printf("\nA3 * v3 = ");
  log_vector_3d(multiply_matrix_3x3_v3(&A3, v3));

  multiply_matrix_3x3(&A3, &B3, &P3);
  printf("\nA3 * B3 = \n");
  log_matrix_3x3(P3);



  //
  // ======== 4x4 MATRIX TESTING ========
  //
  matrix_4x4 A4;
  matrix_4x4 B4;
  matrix_4x4 P4;
  vector_4d v4 = { 2, 4, 6, 1 };

  initialise_identity_matrix_4x4(&A4);
  initialise_null_matrix_4x4(&B4);

  printf("\nMatrix A4 (identity):\n");
  log_matrix_4x4(A4);

  printf("Matrix B4 (null):\n");
  log_matrix_4x4(B4);

  printf("\nSetting some elements in B4...\n");
  set_element_matrix_4x4(&B4, 0, 0, 10);
  set_element_matrix_4x4(&B4, 1, 2, 7.25f);

  printf("B4 after changes:\n");
  log_matrix_4x4(B4);

  printf("\nA4 * v4 = ");
  log_vector_4d(multiply_matrix_4x4_v4(&A4, v4));

  multiply_matrix_4x4(&A4, &B4, &P4);
  printf("\nA4 * B4 = \n");
  log_matrix_4x4(P4);



  //
  // ======== N×M MATRIX TESTING ========
  //
  
  matrix_n_m N1;
  matrix_n_m N2;
  matrix_n_m NP;

  N1.rows = 3; N1.columns = 3;
  N2.rows = 3; N2.columns = 3;
  NP.rows = 3; NP.columns = 3;

  float data_N1[9];
  float data_N2[9];
  float data_NP[9];
  N1.data = data_N1;
  N2.data = data_N2;
  NP.data = data_NP;

  initialise_identity_matrix_n_m(&N1);
  initialise_null_matrix_n_m(&N2);

  printf("\nMatrix N1 (identity 3×3):\n");
  log_matrix_n_m(N1);

  printf("Matrix N2 (null 3×3):\n");
  log_matrix_n_m(N2);

  set_element_matrix_n_m(&N2, 0, 2, 9.9f);
  set_element_matrix_n_m(&N2, 2, 1, -4.5f);

  printf("\nN2 after changes:\n");
  log_matrix_n_m(N2);

  multiply_matrix_n_m(&N1, &N2, &NP);

  printf("\nN1 * N2 = \n");
  log_matrix_n_m(NP);



  //
  // ======== ROTATION MATRIX TESTS ========
  //
  matrix_4x4 R1;
  matrix_4x4 R2;
  matrix_4x4 R3;

  initialise_identity_matrix_4x4(&R1);
  printf("\nRotation X 45deg:\n");
  update_x_rotation_matrix(&R1, 3.1415926f / 4);
  log_matrix_4x4(R1);

  initialise_identity_matrix_4x4(&R2);
  printf("\nRotation Y 60deg:\n");
  update_y_rotation_matrix(&R1, 3.1415926f / 3);
  log_matrix_4x4(R1);

  initialise_identity_matrix_4x4(&R3);
  printf("\nRotation Z 90deg:\n");
  update_z_rotation_matrix(&R3, 3.1415926f / 2);
  log_matrix_4x4(R3);



  //
  // ======== TRIANGLE TESTING ========
  //
  vector_2d ta = {0,0};
  vector_2d tb = {5,0};
  vector_2d tc = {2,4};
  vector_2d tp1 = {2,1};
  vector_2d tp2 = {6,1};
  float c1 = cross_product_vector_2d(subtract_vector_2d(tb, ta),
                                   subtract_vector_2d(tp1, ta));
  float c2 = cross_product_vector_2d(subtract_vector_2d(tc, tb),
                                   subtract_vector_2d(tp1, tb));
  float c3 = cross_product_vector_2d(subtract_vector_2d(ta, tc),
                                   subtract_vector_2d(tp1, tc));
  printf("tp1 crosses: %f, %f, %f\n", c1, c2, c3);
  printf("\nTriangle point tests:\n");

  printf("tp1 is right of AB? %d\n",
    is_point_right_side_line(ta, tb, tp1));
  printf("tp2 is right of AB? %d\n",
    is_point_right_side_line(ta, tb, tp2));

  printf("tp1 inside triangle? %d\n",
    is_point_in_triangle(ta, tb, tc, tp1));
  printf("tp2 inside triangle? %d\n",
    is_point_in_triangle(ta, tb, tc, tp2));



  //
  // ======== RANDOM GENERATION TESTING ========
  //
  Uint32 random_cols[5];

  printf("\nRandom vector fill:\n");

  for (int i = 0; i < 5; i++) {
    printf("vec[%d] = ", i);
  }

  printf("\nRandom colours:\n");
  fill_random_colours(random_cols, 5);

  for (int i = 0; i < 5; i++) {
    printf("colour[%d] = ", i);
    log_colour(random_cols[i]);
  }


  return 0;
}
