#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <string.h>

#include "digitclassifier.h"
#include "getmnist.h"

double activation_neurons[NUM_ACTIVATION_NEURONS];
double activation_weights[NUM_LAYER0_NEURONS][NUM_ACTIVATION_NEURONS];
double activation_biases[NUM_LAYER0_NEURONS];

double layer0_neurons[NUM_LAYER0_NEURONS];
double layer0_weights[NUM_LAYER1_NEURONS][NUM_LAYER0_NEURONS];
double layer0_biases[NUM_LAYER1_NEURONS];

double layer1_neurons[NUM_LAYER1_NEURONS];
double layer1_weights[NUM_OUTPUT_NEURONS][NUM_LAYER1_NEURONS];
double layer1_biases[NUM_OUTPUT_NEURONS];

double output_neurons[NUM_OUTPUT_NEURONS];


int count;

int main(int argc, char *argv[]) {
    struct timeval tv;
    gettimeofday(&tv,NULL);
    srand(tv.tv_usec);


    MnistImage *dataset;
    char* images_loc;
    char* labels_loc;

    if (argc == 5) {
        for (int i = 1; i < argc; i++) {
            if (strcmp(argv[i], "--images-loc") == 0) {
                images_loc = argv[i+1];
            } else if (strcmp(argv[i], "--labels-loc") == 0) {
                labels_loc = argv[i+1];
            }
        }
    } else {
        printf("Not correct args\n");
        return 1;
    }
    dataset = load_mnist(images_loc, labels_loc, &count);

    backpropagation(dataset);
    test(dataset[0]);

    return 0;
}

void compute_network() {
    for (int i = 0; i < NUM_LAYER0_NEURONS; i++) {
        layer0_neurons[i] = logistic(dot(activation_neurons, activation_weights[i], NUM_ACTIVATION_NEURONS) + activation_biases[i]);
    }
    for (int i = 0; i < NUM_LAYER1_NEURONS; i++) {
        layer1_neurons[i] = logistic(dot(layer0_neurons, layer0_weights[i], NUM_LAYER0_NEURONS) + layer0_biases[i]);
    }
    for (int i = 0; i < NUM_OUTPUT_NEURONS; i++) {
        output_neurons[i] = logistic(dot(layer1_neurons, layer1_weights[i], NUM_LAYER1_NEURONS) + layer1_biases[i]);
    }
}

double logistic(double x) { return 1.0 / (1.0 + exp(-x)); }
double dlogistic(double x) { return exp(x) / (pow(1+exp(x), 2)); }

double dot(double* vector0, double* vector1, int vector_length) {
    double sum = 0;

    for (int i = 0; i < vector_length; i++) {
        sum += vector0[i] * vector1[i];
    }

    return sum;
}

int read_output() {
    int highest = 0;
    for (int i = 0; i < NUM_OUTPUT_NEURONS; i++) {
        if (output_neurons[i] > output_neurons[highest]) {
            highest = i;
        }
    }

    return highest;
}

void randomize_network() {
    for (int y = 0; y < NUM_LAYER0_NEURONS; y++) {
        for (int x = 0; x < NUM_ACTIVATION_NEURONS; x++) {
                        activation_weights[y][x] = (double) rand() / (double) RAND_MAX;
        }
        activation_biases[y] = ((double) rand() / (double) RAND_MAX) * NUM_LAYER0_NEURONS;
    }
    for (int y = 0; y < NUM_LAYER1_NEURONS; y++) {
        for (int x = 0; x < NUM_LAYER0_NEURONS; x++) {
            layer0_weights[y][x] = (double) rand() / (double) RAND_MAX;
        }
        layer0_biases[y] = ((double) rand() / (double) RAND_MAX) * NUM_LAYER1_NEURONS;

    }
    for (int y = 0; y < NUM_OUTPUT_NEURONS; y++) {
        for (int x = 0; x < NUM_LAYER1_NEURONS; x++) {
            layer1_weights[y][x] = (double) rand() / (double) RAND_MAX;
        }
        layer1_biases[y] = ((double) rand() / (double) RAND_MAX) * NUM_OUTPUT_NEURONS;
    }
}

void randomize_activations() {
    for (int i = 0; i < NUM_ACTIVATION_NEURONS; i++) {
        activation_neurons[i] = (double) rand() / (double) RAND_MAX;
    }
}

double cost(MnistImage image) {
    double correct[NUM_OUTPUT_NEURONS];
    for (int i = 0; i < NUM_OUTPUT_NEURONS; i++) {
        correct[i] = (double) 0;
    }
    correct[image.digit] = (double) 1;

    double sum = (double) 0;
    for (int i = 0; i < NUM_OUTPUT_NEURONS; i++) {
        sum += pow((output_neurons[i] - correct[i]), 2.0);
    }

    return sum;
}

void backpropagate(MnistImage image) {
    double correct[NUM_OUTPUT_NEURONS];
    for (int i = 0; i < NUM_OUTPUT_NEURONS; i++) {
        correct[i] = (double) 0;
    }
    correct[image.digit] = (double) 1;

    for (int h = 0; h < NUM_LAYER1_NEURONS; h++) {
        for (int i = 0; i < NUM_OUTPUT_NEURONS; i++) {
            layer1_weights[i][h] -= layer1_neurons[h] * dlogistic(dot(layer1_neurons, layer1_weights[i], NUM_LAYER1_NEURONS) + layer1_biases[i]) * 2 * (output_neurons[i] - correct[i]);
            layer1_biases[i] -= dlogistic(dot(layer1_neurons, layer1_weights[i], NUM_LAYER1_NEURONS) + layer1_biases[i]) * 2 * (output_neurons[i] - correct[i]);
        }
    }

    for (int h = 0; h < NUM_LAYER0_NEURONS; h++) {
        for (int i = 0; i < NUM_LAYER1_NEURONS; i++) {
            layer0_weights[i][h] -= layer0_neurons[h] * dlogistic(dot(layer0_neurons, layer0_weights[i], NUM_LAYER0_NEURONS) + layer0_biases[i]) * 2 * (layer1_neurons[i] - output_neurons[i]);
            layer0_biases[i] -= dlogistic(dot(layer0_neurons, layer0_weights[i], NUM_LAYER0_NEURONS) + layer0_biases[i]) * 2 * (layer1_neurons[i] - output_neurons[i]);
        }
    }

    for (int h = 0; h < NUM_ACTIVATION_NEURONS; h++) {
        for (int i = 0; i < NUM_LAYER0_NEURONS; i++) {
            activation_weights[i][h] -= activation_neurons[h] * dlogistic(dot(activation_neurons, activation_weights[i], NUM_ACTIVATION_NEURONS) - activation_biases[i]) * 2 * (layer0_neurons[i] - layer1_neurons[i]);
            activation_biases[i] -= dlogistic(dot(activation_neurons, activation_weights[i], NUM_ACTIVATION_NEURONS) + activation_biases[i]) * 2 * (layer0_neurons[i] - layer1_neurons[i]);
        }
    }
}

void load_backpropagate(MnistImage image) {
    imgcpy(activation_neurons, image.image);
    compute_network();
    backpropagate(image);
    printf("Cost: %lf\n", cost(image));
}

void backpropagation(MnistImage* images) {
    randomize_network();
    for (int i = 0; i < 1000; i++) {
        int random_image = rand() % count;
        printf("Iteration %d, ", i);
        load_backpropagate(images[random_image]);
    }
}

void test(MnistImage image) {
    imgcpy(activation_neurons, image.image);
    compute_network();


    printf("Output layer:\n");
    for (int i = 0; i < NUM_OUTPUT_NEURONS; i++) {
        printf("    %d: %lf\n", i, output_neurons[i]);
    }

    printf("Correct number: %d\n", image.digit);
    printf("Predicted number: %d\n", read_output());
    printf("Cost: %lf\n", cost(image));
}
