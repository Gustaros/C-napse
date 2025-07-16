#include <stdio.h>
#include <string.h>
#include <math.h>
#include "tinyexpr.h" // take from https://github.com/codeplea/tinyexpr

// Глобальная переменная для хранения значения 'x'
double x_value;

// Функция для вычисления производной
// expression_string - формула, введённая пользователем
// x - точка, в которой ищем производную
// h - малый шаг
double calculate_derivative(const char* expression_string, double x, double h) {
    // Определяем переменную 'x', которую будет использовать TinyExpr
    te_variable vars[] = {{"x", &x_value}};

    int err;
    // Компилируем выражение один раз для проверки на синтаксические ошибки
    te_expr *expr_check = te_compile(expression_string, vars, 1, &err);
    if (!expr_check) {
        printf("Ошибка в формуле на позиции %d\n", err);
        return NAN; // NAN (Not-a-Number) - хороший способ вернуть ошибку
    }
    te_free(expr_check);

    // Вычисляем f(x+h)
    x_value = x + h; // Устанавливаем значение 'x' для TinyExpr
    te_expr *expr_plus_h = te_compile(expression_string, vars, 1, 0);
    double y_plus_h = te_eval(expr_plus_h);
    te_free(expr_plus_h);

    // Вычисляем f(x-h)
    x_value = x - h; // Устанавливаем новое значение 'x'
    te_expr *expr_minus_h = te_compile(expression_string, vars, 1, 0);
    double y_minus_h = te_eval(expr_minus_h);
    te_free(expr_minus_h);

    // Формула центральной разности
    return (y_plus_h - y_minus_h) / (2 * h);
}


int main() {
    char formula[256];
    double x_point;
    double h = 1e-6; // Малый шаг для точности

    // 1. Получаем формулу от пользователя
    printf("Введите формулу (используйте 'x' как переменную).\n");
    printf("Примеры: x^2, sin(x)*exp(x), log(x)/x\n> ");
    fgets(formula, sizeof(formula), stdin);

    // fgets считывает символ новой строки '\n', его нужно убрать
    formula[strcspn(formula, "\n")] = 0;

    // 2. Получаем точку для вычисления
    printf("Введите точку x, в которой нужно найти производную: ");
    scanf("%lf", &x_point);

    // 3. Вычисляем производную
    double derivative = calculate_derivative(formula, x_point, h);

    // 4. Выводим результат
    if (!isnan(derivative)) {
        printf("\nПроизводная функции f(x) = %s\n", formula);
        printf("в точке x = %.4f примерно равна: %.8f\n", x_point, derivative);
    } else {
        printf("Не удалось вычислить производную из-за ошибки в выражении.\n");
    }

    return 0;
}