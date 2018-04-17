#include <iostream>

int main() {
    double *expected = new double[50];
    expected[0] = 1;
    double **in = new double*[50];

    expected[20] = 5;
    std::cout<<sizeof(in) / sizeof(in[0]) << " " << sizeof(expected) / sizeof(*expected);
    return 0;
}

