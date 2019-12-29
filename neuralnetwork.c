#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"





void train_network (FILE *fp, const size_t i_m, const size_t h_m, const size_t o_m,
        double W1[i_m][h_m], double W2[h_m][o_m])
{


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
    double W1[input_m][hidden_m];
    double W2[hidden_m][output_m];

    for (int i = 1; i < argc; i++) {
        FILE *fp = fopen(argv[i], "r");
        train_network(fp, i_m, h_m, o_m, W1, W2);
        fclose(fp);
    }


    // TODO: save w1 and w2 to csv file

    return 0;
}
