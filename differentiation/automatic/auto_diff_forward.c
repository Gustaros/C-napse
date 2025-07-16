#include <stdio.h>
#include <math.h>

// Структура для хранения значения (val) и производной (der)
typedef struct {
    double val; // значение f(x)
    double der; // значение f'(x)
} DualNumber;

// Операции для двойных чисел

// Сложение: (a+b)' = a' + b'
DualNumber add(DualNumber a, DualNumber b) {
    DualNumber result;
    result.val = a.val + b.val;
    result.der = a.der + b.der;
    return result;
}

// Умножение: (a*b)' = a'b + ab'
DualNumber multiply(DualNumber a, DualNumber b) {
    DualNumber result;
    result.val = a.val * b.val;
    result.der = a.der * b.val + a.val * b.der;
    return result;
}

// sin(a(x))' = cos(a(x)) * a'(x)
DualNumber sin_dual(DualNumber a) {
    DualNumber result;
    result.val = sin(a.val);
    result.der = cos(a.val) * a.der;
    return result;
}

// Наша функция f(x) = x^2 * sin(x), переписанная для двойных чисел
// x^2 можно представить как x*x
DualNumber f_dual(DualNumber x) {
    DualNumber x_squared = multiply(x, x);
    DualNumber sin_x = sin_dual(x);
    return multiply(x_squared, sin_x);
}

int main() {
    double x_val = 2.0; // Точка, в которой ищем производную

    // "Затравка" (seed) для прямого режима АД
    // Мы ищем производную по x, поэтому d(x)/d(x) = 1
    DualNumber x_dual;
    x_dual.val = x_val;
    x_dual.der = 1.0;   // Это ключевой момент!

    // Вычисляем функцию с помощью наших операций
    DualNumber result = f_dual(x_dual);
    
    // Для сравнения
    double true_deriv = 2 * x_val * sin(x_val) + pow(x_val, 2) * cos(x_val);

    printf("Точка x = %.2f\n", x_val);
    printf("Автоматическое дифференцирование (прямой режим):\n");
    printf("  - Значение функции f(x): %f\n", result.val);
    printf("  - Производная f'(x):   %f\n", result.der);
    printf("Точная производная:         %f\n", true_deriv);
    printf("Погрешность:                %e\n", fabs(result.der - true_deriv));

    return 0;
}