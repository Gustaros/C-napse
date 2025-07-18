#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "activation_f.h"

// --- Функции активации ---

// Сигмоида
double sigmoid(double x) {
    return 1.0 / (1.0 + exp(-x));
}

// Производная сигмоиды (нужна для backpropagation)
double sigmoid_derivative(double x) {
    return x * (1.0 - x);
}