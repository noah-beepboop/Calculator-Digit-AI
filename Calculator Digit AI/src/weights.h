#ifndef WEIGHTS_H
#define WEIGHTS_H

#define INPUT_SIZE 784
#define HIDDEN_SIZE 8
#define OUTPUT_SIZE 10

extern const float input_weights[INPUT_SIZE * HIDDEN_SIZE];
extern const float input_biases[HIDDEN_SIZE];
extern const float output_weights[HIDDEN_SIZE * OUTPUT_SIZE];
extern const float output_biases[OUTPUT_SIZE];

#endif // WEIGHTS_H