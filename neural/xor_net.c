#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "activation_f.h"
#include "config.h"


// --- Глобальные переменные для сети ---

// Веса и смещения
double hidden_weights[NUM_INPUTS][NUM_HIDDEN_NODES];
double output_weights[NUM_HIDDEN_NODES][NUM_OUTPUTS];
double hidden_bias[NUM_HIDDEN_NODES];
double output_bias[NUM_OUTPUTS];

// Активации нейронов
double hidden_layer_output[NUM_HIDDEN_NODES];
double output_layer_output[NUM_OUTPUTS];

// --- Функции для работы сети ---

// Инициализация весов случайными значениями
void initialize_weights() {
    for (int i = 0; i < NUM_INPUTS; i++) {
        for (int j = 0; j < NUM_HIDDEN_NODES; j++) {
            hidden_weights[i][j] = ((double)rand() / RAND_MAX) * 2.0 - 1.0; // от -1 до 1
        }
    }
    for (int i = 0; i < NUM_HIDDEN_NODES; i++) {
        for (int j = 0; j < NUM_OUTPUTS; j++) {
            output_weights[i][j] = ((double)rand() / RAND_MAX) * 2.0 - 1.0; // от -1 до 1
        }
        hidden_bias[i] = ((double)rand() / RAND_MAX) * 2.0 - 1.0;
    }
    for (int i = 0; i < NUM_OUTPUTS; i++) {
        output_bias[i] = ((double)rand() / RAND_MAX) * 2.0 - 1.0;
    }
}

// Прямое распространение (Feedforward)
void feed_forward(double inputs[NUM_INPUTS]) {
    // 1. Вычисление выхода скрытого слоя
    for (int j = 0; j < NUM_HIDDEN_NODES; j++) {
        double activation = hidden_bias[j];
        for (int i = 0; i < NUM_INPUTS; i++) {
            activation += inputs[i] * hidden_weights[i][j];
        }
        hidden_layer_output[j] = sigmoid(activation);
    }

    // 2. Вычисление выхода выходного слоя
    for (int j = 0; j < NUM_OUTPUTS; j++) {
        double activation = output_bias[j];
        for (int i = 0; i < NUM_HIDDEN_NODES; i++) {
            activation += hidden_layer_output[i] * output_weights[i][j];
        }
        output_layer_output[j] = sigmoid(activation);
    }
}

// Обратное распространение ошибки (Backpropagation)
void backpropagate(double inputs[NUM_INPUTS], double expected_output[NUM_OUTPUTS]) {
    // 1. Вычисление ошибки и дельты для выходного слоя
    double output_deltas[NUM_OUTPUTS];
    for (int j = 0; j < NUM_OUTPUTS; j++) {
        double error = expected_output[j] - output_layer_output[j];
        output_deltas[j] = error * sigmoid_derivative(output_layer_output[j]);
    }

    // 2. Вычисление ошибки и дельты для скрытого слоя
    double hidden_deltas[NUM_HIDDEN_NODES];
    for (int j = 0; j < NUM_HIDDEN_NODES; j++) {
        double error = 0.0;
        for (int k = 0; k < NUM_OUTPUTS; k++) {
            error += output_deltas[k] * output_weights[j][k];
        }
        hidden_deltas[j] = error * sigmoid_derivative(hidden_layer_output[j]);
    }

    // 3. Обновление весов и смещений для выходного слоя
    for (int j = 0; j < NUM_OUTPUTS; j++) {
        output_bias[j] += output_deltas[j] * LEARNING_RATE;
        for (int i = 0; i < NUM_HIDDEN_NODES; i++) {
            output_weights[i][j] += hidden_layer_output[i] * output_deltas[j] * LEARNING_RATE;
        }
    }

    // 4. Обновление весов и смещений для скрытого слоя
    for (int j = 0; j < NUM_HIDDEN_NODES; j++) {
        hidden_bias[j] += hidden_deltas[j] * LEARNING_RATE;
        for (int i = 0; i < NUM_INPUTS; i++) {
            hidden_weights[i][j] += inputs[i] * hidden_deltas[j] * LEARNING_RATE;
        }
    }
}

// --- Главная функция ---

int main() {
    // Обучающие данные для XOR
    double training_inputs[NUM_TRAINING_SETS][NUM_INPUTS] = {
        {0.0, 0.0},
        {0.0, 1.0},
        {1.0, 0.0},
        {1.0, 1.0}
    };
    double training_outputs[NUM_TRAINING_SETS][NUM_OUTPUTS] = {
        {0.0},
        {1.0},
        {1.0},
        {0.0}
    };

    // Инициализация генератора случайных чисел
    srand(time(NULL));

    // Инициализация весов
    initialize_weights();

    printf("Начало обучения...\n");

    // Цикл обучения
    for (int epoch = 0; epoch < NUM_EPOCHS; epoch++) {
        double total_error = 0;
        for (int i = 0; i < NUM_TRAINING_SETS; i++) {
            // Прямое распространение
            feed_forward(training_inputs[i]);

            // Обратное распространение
            backpropagate(training_inputs[i], training_outputs[i]);

            // Подсчет ошибки (для вывода)
            for(int j=0; j<NUM_OUTPUTS; j++) {
                total_error += fabs(training_outputs[i][j] - output_layer_output[j]);
            }
        }
        // Выводим среднюю ошибку каждые 1000 эпох
        if ((epoch + 1) % 1000 == 0) {
            printf("Эпоха %d, Средняя ошибка: %f\n", epoch + 1, total_error / NUM_TRAINING_SETS);
        }
    }

    printf("Обучение завершено!\n\n");

    // Тестирование сети
    printf("Результаты после обучения:\n");
    for (int i = 0; i < NUM_TRAINING_SETS; i++) {
        feed_forward(training_inputs[i]);
        printf("Вход: %.1f, %.1f  ->  Выход: %f (Ожидаемый: %.1f)\n",
               training_inputs[i][0], training_inputs[i][1],
               output_layer_output[0], training_outputs[i][0]);
    }

    return 0;
}