#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"





void train_network (FILE *fp, const size_t i_m, const size_t h_m, const size_t o_m,
        double W1[i_m][h_m], double W2[h_m][o_m])
{
    char buffer[33];

    double inputs[i_m][1];

    // skip the first lines
    do {
        fgets(buffer, 33, fp);
    } while (buffer[0] > '@');

    // line counter
    int line_c = 0;

    for (int i = 0; i < 16; i++)
        inputs[i][0] = 0;

    do {
        if (line_c == 32) {
            // TODO: multiplication;

            char b[3];
            fgets(b, 3, fp);
            int expected_val = b[1];

            doubles_matrix_println(16, 1, inputs);
            printf("%d\n", expected_val);

            // TODO: regression;

            line_c = 0;
            
            // reset input vector
            for (int i = 0; i < 16; i++)
                inputs[i][0] = 0;
        } else {
            // update the input vector
            for (int i = 0; i < 32; i++)
                if (buffer[i] == '1') {
                    int col = i >> 2;
                    int row = line_c >> 2;
                    inputs[8*row + col][0] = 1;
                }
            line_c++;
        }
    } while (fgets(buffer, 32, fp) != EOF && line_c > 0); // TODO: remove line later

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
    double W1[i_m][h_m];
    double W2[h_m][o_m];

    for (int i = 1; i < argc; i++) {
        FILE *fp = fopen(argv[i], "r");
        train_network(fp, i_m, h_m, o_m, W1, W2);
        fclose(fp);
    }


    // TODO: save w1 and w2 to csv file

    return 0;
}
