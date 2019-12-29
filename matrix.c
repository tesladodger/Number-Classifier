#include <stdio.h>
#include <stdlib.h>


void doubles_matrix_multiplication
(const size_t m, const size_t n, const size_t p, 
 const double A[m][n], const double B[n][p], double C[m][p])
{
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < p; j++) {
            C[i][j] = 0.0;
            for (int k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}


void doubles_matrix_println (const size_t m, const size_t n, const double A[m][n])
{
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            printf("%-14.5lf", A[i][j]);
        }
        printf("\n");
    }
}



// ---------------- Tests ---------------- //

void test_doubles_matrix_multiplication ()
{
    const double A[3][2] = {
        {0, 2},
        {5, 3},
        {1, 5}
    };
    const double B[2][4] = {
        {3, 7, 8, 9},
        {9, 2, 0, 3}
    };

    double C[3][4];

    doubles_matrix_multiplication(3, 2, 4, A, B, C);
    doubles_matrix_println(3, 4, C);
}


void test_doubles_matrix_println ()
{
    double A[3][2] = {
        {0.5, 2.4},
        {6.9, 1.2},
        {1.0, 0.0}
    };
    doubles_matrix_println(3, 2, A);
}
