#include <stdio.h>

int main() {
    int son = 18;
    int father = 45;
    int younger;

    younger = son < father ? son : father;
    printf("Youngest age is: %d\n", younger);
    return 0;
}