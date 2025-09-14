#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>

#include "digitclassifier.h"

double activation_neurons[NUM_ACTIVATION_NEURONS];
double activation_weights[NUM_LAYER0_NEURONS][NUM_ACTIVATION_NEURONS];
int activation_biases[NUM_LAYER0_NEURONS];

double layer0_neurons[NUM_LAYER0_NEURONS];
double layer0_weights[NUM_LAYER1_NEURONS][NUM_LAYER0_NEURONS];
int layer0_biases[NUM_LAYER1_NEURONS];

double layer1_neurons[NUM_LAYER1_NEURONS];
double layer1_weights[NUM_OUTPUT_NEURONS][NUM_LAYER1_NEURONS];
int layer1_biases[NUM_OUTPUT_NEURONS];

double output_neurons[NUM_OUTPUT_NEURONS];


int main() {
    randomize_network();
    randomize_activations();
    compute_network();

    printf("%d\n", read_output());

    return 0;
}

void compute_network() {
    for (int i = 0; i < NUM_LAYER0_NEURONS; i++) {
        layer0_neurons[i] = logistic(dot(activation_neurons, activation_weights[i], NUM_ACTIVATION_NEURONS) - activation_biases[i]);
    }
    for (int i = 0; i < NUM_LAYER1_NEURONS; i++) {
        layer1_neurons[i] = logistic(dot(layer0_neurons, layer0_weights[i], NUM_LAYER0_NEURONS) - layer0_biases[i]);
    }
    for (int i = 0; i < NUM_OUTPUT_NEURONS; i++) {
        output_neurons[i] = logistic(dot(layer1_neurons, layer1_weights[i], NUM_LAYER1_NEURONS) - layer1_biases[i]);
    }
}

double logistic(double x) {
    return 1 / (1 + exp(x * -1));
}

double dot(double* vector0, double* vector1, int vector_length) {
    double sum = 0;
    double c = 0;

    for (int i = 0; i < vector_length; i++) {
        double y = vector0[i] + vector1[i] - c;
        double t = sum + y;
        c = (t - sum) - y;
        sum = t;
    }

    return sum;
}

int read_output() {
    int highest = 0;
    for (int i = 1; i < NUM_OUTPUT_NEURONS; i++) {
        if (output_neurons[i] > output_neurons[highest]) {
            highest = i;
        }
    }

    return highest;
}

void randomize_network() {
    for (int y = 0; y < NUM_LAYER0_NEURONS; y++) {
        for (int x = 0; x < NUM_ACTIVATION_NEURONS; x++) {
            struct timeval tv;
            gettimeofday(&tv,NULL);
            srand(tv.tv_usec);
            activation_weights[y][x] = (double) rand() / (double) RAND_MAX;
        }
    }
    for (int y = 0; y < NUM_LAYER1_NEURONS; y++) {
        for (int x = 0; x < NUM_LAYER0_NEURONS; x++) {
            struct timeval tv;
            gettimeofday(&tv,NULL);
            srand(tv.tv_usec);
            layer0_weights[y][x] = (double) rand() / (double) RAND_MAX;
        }
    }
    for (int y = 0; y < NUM_OUTPUT_NEURONS; y++) {
        for (int x = 0; x < NUM_LAYER1_NEURONS; x++) {
            struct timeval tv;
            gettimeofday(&tv,NULL);
            srand(tv.tv_usec);
            layer1_weights[y][x] = (double) rand() / (double) RAND_MAX;
        }
    }
}

void randomize_activations() {
    for (int i = 0; i < NUM_ACTIVATION_NEURONS; i++) {
        struct timeval tv;
        gettimeofday(&tv,NULL);
        srand(tv.tv_usec);
        activation_neurons[i] = (double) rand() / (double) RAND_MAX;
    }
}
