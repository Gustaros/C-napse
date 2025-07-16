#include <stdio.h>
#include <math.h>

// Наша функция: f(x) = x^2 * sin(x)
double f(double x) {
    return pow(x, 2) * sin(x);
}

// Аналитическая производная для проверки: f'(x) = 2x*sin(x) + x^2*cos(x)
double f_derivative_true(double x) {
    return 2 * x * sin(x) + pow(x, 2) * cos(x);
}

// Численная производная (центральная разность - более точная)
// func - указатель на функцию, которую мы дифференцируем
double numerical_derivative(double (*func)(double), double x, double h) {
    return (func(x + h) - func(x - h)) / (2 * h);
}

int main() {
    double x = 2.0;       // Точка, в которой ищем производную
    double h = 1e-6;      // Маленький шаг

    // Вычисляем производную численно
    double num_deriv = numerical_derivative(f, x, h);

    // Вычисляем точное значение для сравнения
    double true_deriv = f_derivative_true(x);

    printf("Точка x = %.2f\n", x);
    printf("Численная производная: %f\n", num_deriv);
    printf("Точная производная:     %f\n", true_deriv);
    printf("Погрешность:            %e\n", fabs(num_deriv - true_deriv));

    return 0;
}