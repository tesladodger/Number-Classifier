#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "matrix.h"

#define LEARNING_RATE 0.1


double sigmoid_transfer_function (double x)
{
    return 1 / ( 1 + pow(M_E, -4.9*x) );
}


void softmax_function (const size_t n, double a[1][n])
{
    double sum = 0;
    for (int i = 0; i < n; i++)
        sum += pow(M_E, a[0][i]);

    for (int i = 0; i < n; i++)
        a[0][i] = pow(M_E, a[0][i]) / sum;
}


void feed_forward (const size_t i_n, const size_t h_n, const size_t o_n,
        const double W1[i_n][h_n], const double W2[h_n][o_n],
        const double B1[1][h_n], const double B2[1][o_n],
        double inputs[1][i_n], double hiddens[1][h_n], double outputs[1][o_n])
{
    // input normalization (what fraction of the squared is covered)
    for (int i = 0; i < i_n; i++)
        inputs[0][i] /= 32;

    // calculate the hidden layer
    doubles_matrix_multiplication(1, i_n, h_n, inputs, W1, hiddens);
    doubles_matrix_add_to(1, h_n, hiddens, B1);

    // apply the activation function to the hidden layer
    for (int i = 0; i < h_n; i++)
        hiddens[0][i] = sigmoid_transfer_function(hiddens[0][i]);

    // calculate the output layer
    doubles_matrix_multiplication(1, h_n, o_n, hiddens, W2, outputs);
    doubles_matrix_add_to(1, o_n, outputs, B2);

    // apply the transfer function to the outputs
    softmax_function(o_n, outputs);
}


void backpropagation (const size_t i_n, const size_t h_n, const size_t o_n,
        double W1[i_n][h_n], double W2[h_n][o_n], double B1[1][h_n], double B2[1][o_n],
        const double outputs[1][o_n], const double hiddens[1][h_n], const double inputs[1][i_n],
        const int expected_value)
{
    // output error vector
    double o_errors[o_n][1];
    for (int i = 0; i < o_n; i++)
        o_errors[i][0] = (i == expected_value) ? 1 - outputs[0][i] : 0 - outputs[0][i];

    // hidden layer error vector
    double h_errors[h_n][1];
    doubles_matrix_multiplication(h_n, o_n, 1, W2, o_errors, h_errors);

    // bias vectors correction
    for (int i = 0; i < o_n; i++)
        B2[0][i] += LEARNING_RATE * o_errors[i][0] * outputs[0][i] * (1.0 - outputs[0][i]);
    for (int i = 0; i < h_n; i++)
        B1[0][i] += LEARNING_RATE * h_errors[i][0] * hiddens[0][i] * (1.0 - hiddens[0][i]);

    // deltas for the weights from hiddens to outputs
    double tem0[o_n][1];
    for (int i = 0; i < o_n; i++)
        tem0[i][0] = LEARNING_RATE * o_errors[i][0] * outputs[0][i] * (1.0 - outputs[0][i]);
    double delta_W2_t[o_n][h_n];
    doubles_matrix_multiplication(o_n, 1, h_n, tem0, hiddens, delta_W2_t);

    // deltas for the weights from inputs to hiddens
    double tem1[h_n][1];
    for (int i = 0; i < h_n; i++)
        tem1[i][0] = LEARNING_RATE * h_errors[i][0] * hiddens[0][i] * (1.0 - hiddens[0][i]);
    double delta_W1_t[h_n][i_n];
    doubles_matrix_multiplication(h_n, 1, i_n, tem1, inputs, delta_W1_t);

    // add the deltas to the weights
    for (int i = 0; i < h_n; i++)
        for (int j = 0; j < o_n; j++)
            W2[i][j] += delta_W2_t[j][i];

    for (int i = 0; i < i_n; i++)
        for (int j = 0; j < h_n; j++)
            W1[i][j] += delta_W1_t[j][i];
}


void train_network (FILE *fp, const size_t i_n, const size_t h_n, const size_t o_n,
        double W1[i_n][h_n], double W2[h_n][o_n], double B1[1][h_n], double B2[1][o_n])
{
    char buffer[40];

    double inputs[1][i_n];

    do {
        fgets(buffer, 40, fp);
    } while (buffer[0] > '@');

    int line_c = 0;

    for (int i = 0; i < i_n; i++)
        inputs[0][i] = 0;

    int counter = 0;

    do {
        if (line_c == 32) {

            double outputs[1][o_n];
            double hiddens[1][h_n];
            feed_forward(i_n, h_n, o_n, W1, W2, B1, B2, inputs, hiddens, outputs);

            int prediction = doubles_vector_find_max_index(o_n, outputs);

            // the expected result is in the next line buffer
            int x = buffer[1] - 48;

            printf("%4d: %d -> %d", counter, x, prediction);
            printf(prediction == x ? "  CORRECT\n" : "\n");

            backpropagation(i_n, h_n, o_n, W1, W2, B1, B2, outputs, hiddens, inputs, x);

            line_c = 0;
            counter++;
            
            for (int i = 0; i < i_n; i++)
                inputs[0][i] = 0;

        } else {
            // update the input vector
            for (int i = 0; i < 32; i++) {
                if (buffer[i] == '1') {
                    int col = i >> 2;
                    int row = line_c >> 2;
                    inputs[0][8*row + col] += 1;
                }
            }
            line_c++;
        }
    } while (fgets(buffer, 40, fp) != NULL);

}


int main (int argc, char *argv[])
{
    // sizes of the input, hidden and output vectors
    const size_t i_n = 64;
    const size_t h_n = 32;
    const size_t o_n = 10;

    // matrices for the weights of the connections
    // W1 -> weights from input to hidden
    // W2 -> weights from hidden to output
    double W1[i_n][h_n];
    double W2[h_n][o_n];
    for (int i = 0; i < i_n; i++)
        for (int j = 0; j < h_n; j++)
            W1[i][j] = (float) random() / RAND_MAX - .5;
    for (int i = 0; i < h_n; i++)
        for (int j = 0; j < o_n; j++)
            W2[i][j] = (float) random() / RAND_MAX - .5;

    // bias vectors
    // B1 -> bias to the hidden layer
    // B2 -> bias to the output layer
    double B1[1][h_n];
    double B2[1][o_n];
    for (int i = 0; i < h_n; i++)
        B1[0][i] = (float) random() / RAND_MAX - .5;
    for (int i = 0; i < o_n; i++)
        B2[0][i] = (float) random() / RAND_MAX - .5;


    for (int i = 1; i < argc; i++) {
        FILE *fp = fopen(argv[i], "r");
        train_network(fp, i_n, h_n, o_n, W1, W2, B1, B2);
        fclose(fp);
    }

    // save the resulting weights to a csv file
    FILE *fp = fopen("../out/nn.csv", "w");
    for (int i = 0; i < i_n; i++) {
        for (int j = 0; j < h_n; j++)
            fprintf(fp, "%.8lf,", W1[i][j]);
        fprintf(fp, "\n");
    }
    for (int i = 0; i < h_n; i++) {
        for (int j = 0; j < o_n; j++)
            fprintf(fp, "%.8lf,", W2[i][j]);
        fprintf(fp, "\n");
    }
    for (int i = 0; i < h_n; i++)
        fprintf(fp, "%.8lf,", B1[0][i]);
    fprintf(fp, "\n");
    for (int i = 0; i < o_n; i++)
        fprintf(fp, "%.8lf,", B2[0][i]);
    fprintf(fp, "\n");
    fclose(fp);

    return 0;
}
