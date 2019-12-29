#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "matrix.h"


double sigmoid_transfer_function (double x)
{
    return 1 / ( 1 + pow(M_E, -4.9*x) );
}


void feed_forward (const size_t i_m, const size_t h_m, const size_t o_m,
        const double W1[h_m][i_m], const double W2[o_m][h_m],
        const double B1[h_m][1], const double B2[o_m][1],
        double inputs[i_m][1], double outputs[o_m][1])
{
    // input normalization (what fraction of the squared is covered)
    for (int i = 0; i < i_m; i++)
        inputs[i][0] /= 64;

    // calculate the hidden layer
    double hidden_layer[h_m][1];
    doubles_matrix_multiplication(h_m, i_m, 1, W1, inputs, hidden_layer);
    doubles_matrix_add_to(h_m, 1, hidden_layer, B1);

    // apply the activation function to the hidden layer
    for (int i = 0; i < h_m; i++)
        hidden_layer[i][0] = sigmoid_transfer_function(hidden_layer[i][0]);

    // calculate the output layer
    doubles_matrix_multiplication(o_m, h_m, 1, W2, hidden_layer, outputs);
    doubles_matrix_add_to(o_m, 1, outputs, B2);

    // apply the transfer function to the outputs
    for (int i = 0; i < o_m; i++)
        outputs[i][0] = sigmoid_transfer_function(outputs[i][0]);
}


void backpropagation (const size_t i_m, const size_t h_m, const size_t o_m,
        double W1[h_m][i_m], double W2[o_m][h_m], double B1[h_m][1], double B2[o_m][1],
        const double outputs[o_m][1], const int expected_value)
{
    int result = doubles_vertical_vector_find_max_index(o_m, outputs);

}


void train_network (FILE *fp, const size_t i_m, const size_t h_m, const size_t o_m,
        double W1[h_m][i_m], double W2[o_m][h_m], double B1[h_m][1], double B2[o_m][1])
{
    char buffer[40];

    double inputs[i_m][1];

    // skip the first lines
    do {
        fgets(buffer, 40, fp);
    } while (buffer[0] > '@');

    // line counter
    int line_c = 0;

    for (int i = 0; i < i_m; i++)
        inputs[i][0] = 0;

    int counter = 0;

    do {
        if (line_c == 32) {

            double outputs[o_m][1];
            feed_forward(i_m, h_m, o_m, W1, W2, B1, B2, inputs, outputs);

            int x = buffer[1] - 48;

            backpropagation(i_m, h_m, o_m, W1, W2, B1, B2, outputs, x);

            line_c = 0;
            counter++;
            
            // reset input vector
            for (int i = 0; i < 16; i++)
                inputs[i][0] = 0;

        } else {
            // update the input vector
            for (int i = 0; i < 32; i++) {
                if (buffer[i] == '1') {
                    int col = i >> 3;
                    int row = line_c >> 3;
                    inputs[4*row + col][0] += 1;
                }
            }
            line_c++;
        }
    } while (fgets(buffer, 40, fp) != NULL); // TODO: remove line later

}


int main (int argc, char *argv[])
{
    // sizes of the input, hidden and output vectors
    const size_t i_m = 16;
    const size_t h_m = 14;
    const size_t o_m = 10;

    // matrices for the weights of the connections
    // W1 -> weights from input to hidden
    // W2 -> weights from hidden to output
    double W1[h_m][i_m];
    double W2[o_m][h_m];
    for (int i = 0; i < h_m; i++)
        for (int j = 0; j < i_m; j++)
            W1[i][j] = .5;
    for (int i = 0; i < o_m; i++)
        for (int j = 0; j < h_m; j++)
            W2[i][j] = .5;

    // bias vectors
    // B1 -> bias to the hidden layer
    // B2 -> bias to the output layer
    double B1[h_m][1];
    double B2[o_m][1];
    for (int i = 0; i < h_m; i++)
        B1[i][0] = .1;
    for (int i = 0; i < o_m; i++)
        B2[i][0] = .2;


    for (int i = 1; i < argc; i++) {
        FILE *fp = fopen(argv[i], "r");
        train_network(fp, i_m, h_m, o_m, W1, W2, B1, B2);
        fclose(fp);
    }

    // save the resulting weights to a csv file
    FILE *fp = fopen("nn.csv", "w");
    for (int i = 0; i < h_m; i++) {
        for (int j = 0; j < i_m; j++)
            fprintf(fp, "%lf,", W1[i][j]);
        fprintf(fp, "\n");
    }
    for (int i = 0; i < o_m; i++) {
        for (int j = 0; j < h_m; j++)
            fprintf(fp, "%lf,", W2[i][j]);
        fprintf(fp, "\n");
    }
    for (int i = 0; i < h_m; i++)
        fprintf(fp, "%lf,", B1[i][0]);
    fprintf(fp, "\n");
    for (int i = 0; i < o_m; i++)
        fprintf(fp, "%lf,", B2[i][0]);
    fprintf(fp, "\n");

    fclose(fp);

    return 0;
}
