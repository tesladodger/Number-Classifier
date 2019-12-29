#ifndef MATRIX_H
#define MATRIX_H


void doubles_matrix_multiplication (const size_t m, const size_t n, const size_t p,
        const double A[m][n], const double B[n][p], double C[m][p]);

void doubles_matrix_add_to (const size_t m, const size_t n, double A[m][n], const double B[m][n]);

int doubles_vertical_vector_find_max_index (const size_t n, const double A[n][1]);

void doubles_matrix_println (const size_t m, const size_t n, const double A[m][n]);






// ---------------- Tests ---------------- //
void test_doubles_matrix_multiplication ();
void test_doubles_matrix_add_to ();
void test_doubles_vertical_vector_find_max_index ();
void test_doubles_matrix_println ();








#endif
