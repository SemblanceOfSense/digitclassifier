#define NUM_ACTIVATION_NEURONS 28 * 28
#define NUM_LAYER0_NEURONS 16
#define NUM_LAYER1_NEURONS 16
#define NUM_OUTPUT_NEURONS 10

void compute_network();

double logistic(double x);

double dot(double* vector0, double* vector1, int vector_length);

int read_output();

void randomize_network();
void randomize_activations();
