#include "getmnist.c"

void compute_network();

double logistic(double x);
double dlogistic(double x);

double dot(double* vector0, double* vector1, int vector_length);

int read_output();

void randomize_network();
void randomize_activations();

double cost(MnistImage image);

void backpropagation(MnistImage* image);
void test(MnistImage image);
