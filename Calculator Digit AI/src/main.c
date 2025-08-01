#include <ti/screen.h>
#include <ti/getcsc.h>
#include <stdlib.h>
#include "weights.h"
#include <math.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <graphx.h>
#include <ti/getkey.h>

int input_rgb[INPUT_SIZE];
int input_1555[INPUT_SIZE];

void set_palette(void) {
    uint16_t grayscale_palette[256];
    for (int i = 0; i < 256; i++) {
        grayscale_palette[i] = gfx_RGBTo1555(i, i, i);
    }
    gfx_SetPalette(grayscale_palette, sizeof(grayscale_palette), 0);
}

float input[INPUT_SIZE];

void fillitup(void) {
    for (int i = 0; i < INPUT_SIZE; i++) {
        input[i] = 0.0f;
    }
}

// Just loop over all the stuff do the math and add it up

void layer(const float *weights, const float *biases, const float *input, float *output, size_t input_size, size_t output_size) {
    for (size_t j = 0; j < output_size; j++) {

        float sum = biases[j]; // You can just do this because it just gets summed either way - the order doesn't matter

        for (size_t i = 0; i < input_size; i++) {

            sum += weights[j * input_size + i] * input[i];

        }
            
        output[j] = sum;
    }
}

float hidden_layer[HIDDEN_SIZE];
float output_layer[OUTPUT_SIZE];

int nn(void) {

    layer(input_weights, input_biases, input, hidden_layer, INPUT_SIZE, HIDDEN_SIZE);
    layer(output_weights, output_biases, hidden_layer, output_layer, HIDDEN_SIZE, OUTPUT_SIZE);

    float largestnum = output_layer[0];
    int largestnumindex = 0;
    for (int i = 1; i < OUTPUT_SIZE; i++) { // Find the neuron with the highest activation
        if (output_layer[i] > largestnum) {
            largestnum = output_layer[i];
            largestnumindex = i;
        }
    }

    return largestnumindex;
}

void draw(void) {
    for (int k = 0; k < INPUT_SIZE; k++) {
        int number = round(input[k] * 255);
        input_rgb[k] = number;
    }

    for (int i = 0; i < 28; i++) {
        for (int j = 0; j < 28; j++) {
            gfx_SetColor(input_rgb[28 * i + j]);
            gfx_FillRectangle(8 + 8 * j, 8 + 8 * i, 7, 7);
        }
    }
}

int pointer_x = 6;
int pointer_y = 6;

void tick(void) {

    // Middle
    input[pointer_y * 28 + pointer_x] += 0.6f;

    // Sides
    input[pointer_y * 28 + pointer_x - 1] += 0.4f;
    input[pointer_y * 28 + pointer_x + 1] += 0.4f; 
    input[(pointer_y - 1) * 28 + pointer_x] += 0.4f;
    input[(pointer_y + 1) * 28 + pointer_x] += 0.4f;

    // Diagonals
    input[(pointer_y - 1) * 28 + pointer_x - 1] += 0.2f;
    input[(pointer_y - 1) * 28 + pointer_x + 1] += 0.2f;
    input[(pointer_y + 1) * 28 + pointer_x - 1] += 0.2f;
    input[(pointer_y + 1) * 28 + pointer_x + 1] += 0.2f;

    for (int i = 0; i < INPUT_SIZE; i++) {
        if (input[i] > 1.0f) {
            input[i] = 1.0f;
        }
    }

    gfx_SetDrawBuffer();
    draw();
    int result = nn();
    gfx_SetColor(255);
    gfx_FillRectangle(260, 100, 30, 30);
    gfx_SetColor(0);
    gfx_SetTextXY(270, 120);
    gfx_PrintInt(result, 0);
    gfx_SwapDraw();
}

int main(void) {

    os_ClrHome();
    gfx_Begin();
    fillitup();
    set_palette();
    tick();

    while (os_GetKey() != 0x40) { // Quit (2nd + Mode)

        uint16_t key = os_GetKey();

        if (key == 0x01) { // Right
            pointer_x += 1;
        } else if (key == 0x02) { // Left
            pointer_x -= 1;
        } else if (key == 0x03) { // Up
            pointer_y -= 1;
        } else if (key == 0x04) { // Down
            pointer_y += 1;
        } else if (key == 0x09) { // Clear
            fillitup();
        }

        tick();
        

    }

    gfx_End();
    return 0;
    
}