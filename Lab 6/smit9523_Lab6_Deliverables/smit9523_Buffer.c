/*
 * File:   smit9523_Lab6.c
 * Author: andreasmith
 *
 * Created on April 16, 2020, 10:19 PM
 */

#include "xc.h"
#include "smit9523_Buffer.h"

#define BUFFER_SIZE 16
    
long buffer[BUFFER_SIZE];    
int buffIdx = 0;

void putVal(int newValue) {
    buffer[buffIdx++] = newValue;
    if (buffIdx >= BUFFER_SIZE) { // Reset if buffer size exceeded
        buffIdx = 0;
    }
}

int getAvg() {
    double sum = 0;
    int i;
    for (i = 0; i < BUFFER_SIZE; i++) {
        sum = sum + buffer[i]; // Add up all the values in the buffer
    }
    
    return sum/BUFFER_SIZE; // Return avg
}

void initBuffer() {
    int i = 0;
    for (i = 0; i < BUFFER_SIZE; i++) {
        buffer[i] = 0;
    }
}