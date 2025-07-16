#include <stdio.h>
#include <stdlib.h>

// Типы узлов в нашем дереве выражений
typedef enum {
    NODE_CONSTANT,
    NODE_VARIABLE,
    NODE_ADD,
    NODE_MULTIPLY
} NodeType;

// Структура, описывающая один узел дерева (выражение)
// Используем forward declaration, т.к. структура ссылается сама на себя
typedef struct Expression Expression;

struct Expression {
    NodeType type;
    union {
        double value; // Для NODE_CONSTANT
        // Для NODE_VARIABLE имя не храним, т.к. у нас только 'x'
        struct {
            Expression* left;
            Expression* right;
        } op; // Для NODE_ADD и NODE_MULTIPLY
    } data;
};

// Создать узел-константу
Expression* create_constant(double value) {
    Expression* expr = (Expression*)malloc(sizeof(Expression));
    expr->type = NODE_CONSTANT;
    expr->data.value = value;
    return expr;
}

// Создать узел-переменную ('x')
Expression* create_variable() {
    Expression* expr = (Expression*)malloc(sizeof(Expression));
    expr->type = NODE_VARIABLE;
    return expr;
}

// Создать узел-операцию (сложение или умножение)
Expression* create_operation(NodeType type, Expression* left, Expression* right) {
    Expression* expr = (Expression*)malloc(sizeof(Expression));
    expr->type = type;
    expr->data.op.left = left;
    expr->data.op.right = right;
    return expr;
}

// Рекурсивная функция для нахождения производной
Expression* differentiate(Expression* expr) {
    if (!expr) return NULL;

    switch (expr->type) {
        // Производная константы: (c)' = 0
        case NODE_CONSTANT:
            return create_constant(0.0);

        // Производная переменной x: (x)' = 1
        case NODE_VARIABLE:
            return create_constant(1.0);

        // Правило суммы: (u + v)' = u' + v'
        case NODE_ADD:
            return create_operation(
                NODE_ADD,
                differentiate(expr->data.op.left),
                differentiate(expr->data.op.right)
            );

        // Правило произведения: (u * v)' = u'v + uv'
        case NODE_MULTIPLY: {
            Expression* u = expr->data.op.left;
            Expression* v = expr->data.op.right;
            Expression* du = differentiate(u); // u'
            Expression* dv = differentiate(v); // v'

            // Создаем два слагаемых: (u'*v) и (u*v')
            Expression* term1 = create_operation(NODE_MULTIPLY, du, v);
            Expression* term2 = create_operation(NODE_MULTIPLY, u, dv);

            // Возвращаем их сумму: (u'*v) + (u*v')
            return create_operation(NODE_ADD, term1, term2);
        }
    }
    return NULL; // На случай непредвиденного типа узла
}

// Рекурсивно печатает выражение
void print_expression(Expression* expr) {
    if (!expr) return;

    switch (expr->type) {
        case NODE_CONSTANT:
            printf("%.1f", expr->data.value);
            break;
        case NODE_VARIABLE:
            printf("x");
            break;
        case NODE_ADD:
            printf("(");
            print_expression(expr->data.op.left);
            printf(" + ");
            print_expression(expr->data.op.right);
            printf(")");
            break;
        case NODE_MULTIPLY:
            printf("(");
            print_expression(expr->data.op.left);
            printf(" * ");
            print_expression(expr->data.op.right);
            printf(")");
            break;
    }
}

// Рекурсивно освобождает память дерева
void free_expression(Expression* expr) {
    if (!expr) return;

    if (expr->type == NODE_ADD || expr->type == NODE_MULTIPLY) {
        free_expression(expr->data.op.left);
        free_expression(expr->data.op.right);
    }
    free(expr);
}


int main() {
    // 1. Создаем дерево для функции f(x) = x * (x + 2)
    // Часть (x + 2)
    Expression* term_x_plus_2 = create_operation(
        NODE_ADD,
        create_variable(),   // x
        create_constant(2.0) // 2
    );
    // Вся функция x * (...)
    Expression* func = create_operation(
        NODE_MULTIPLY,
        create_variable(),   // x
        term_x_plus_2        // (x+2)
    );

    printf("Исходная функция f(x): ");
    print_expression(func);
    printf("\n");

    // 2. Находим производную
    Expression* derivative = differentiate(func);

    printf("Производная f'(x):     ");
    print_expression(derivative);
    printf("\n");

    printf("Ожидаемый результат:    2.0*x + 2.0\n");
    printf("Наш результат без упрощения эквивалентен этому.\n");


    // 3. Очищаем память. ВАЖНО!
    // Обратите внимание, что `derivative` использует части дерева `func`,
    // поэтому мы не можем просто удалить `func` до того, как закончим с `derivative`.
    // В нашем случае, мы можем просто удалить оба дерева в конце.
    // В реальной системе управление памятью было бы сложнее.
    // Для этого примера просто освобождаем результирующее дерево.
    // ВАЖНО: полная очистка памяти в этом примере сложна, т.к. деревья
    // переиспользуют узлы. Простая очистка `derivative` приведет к ошибкам.
    // Для простоты примера мы оставим эту проблему, но в реальном коде
    // потребовалась бы система "умных указателей" или подсчета ссылок.
    
    // Правильная очистка в этом случае будет очень сложной, поэтому мы ее опустим,
    // чтобы не усложнять основной принцип. В такой маленькой программе утечка
    // памяти при завершении не страшна.
    
    // free_expression(func); // Это вызовет ошибку, если сделать перед free(derivative)
    // free_expression(derivative);

    return 0;
}