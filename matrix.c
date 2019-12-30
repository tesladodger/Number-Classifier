#include <stdio.h>
#include <stdlib.h>


/* Multiplies A and B into the result C */
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


/* Places the transpose of A in B */
void doubles_matrix_transpose (const size_t m, const size_t n, const double A[m][n], double B[n][m])
{
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
           B[j][i] = A[i][j];
}


/* Adds B to A */
void doubles_matrix_add_to
(const size_t m, const size_t n, double A[m][n], const double B[m][n])
{
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            A[i][j] += B[i][j];
}


/* Returns the index of the maximum element in a 1*n matrix (row vector) */
int doubles_vector_find_max_index (const size_t n, const double A[1][n])
{
    int result = 0;
    for (int i = 1; i < n; i++)
        if (A[0][i] > A[0][result])
            result = i;
    return result;
}


/* Prints a matrix of doubles */
void doubles_matrix_println (const size_t m, const size_t n, const double A[m][n])
{
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            printf("%-12.5lf", A[i][j]);
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


void test_doubles_matrix_transpose ()
{
    double A[3][5] = {
        {7, 6, 5, 9, 8},
        {0, 9, 5, 6, 3},
        {6, 7, 8, 5, 4}
    };
    double B[5][3];
    doubles_matrix_transpose(3, 5, A, B);
    doubles_matrix_println(5, 3, B);
}


void test_doubles_matrix_add_to ()
{
    double A[2][4] = {
        {1, 2, 3, 4},
        {0, 3, 1, 3}
    };
    const double B[2][4] = {
        {5, 2, 3, 1},
        {0, 3, 1, 2}
    };
    doubles_matrix_add_to(2, 4, A, B);
    doubles_matrix_println(2, 4, A);
}


void test_doubles_vector_find_max_index ()
{
    const double A[1][5] = {{4, 1, 0, 6, 6.1}};
    printf("%d\n", doubles_vector_find_max_index(5, A));
}


void test_doubles_matrix_println ()
{
    const double A[3][2] = {
        {0.5, 2.4},
        {6.9, 1.2},
        {1.0, 0.0}
    };
    doubles_matrix_println(3, 2, A);
}


