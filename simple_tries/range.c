#include <stdio.h>
#include <stdlib.h>

void range_gen(int start, int end, int step) {
    if (step <= 0) {
        printf("Step must be greater than 0.\n");
        return;
    }

    for (int i = start; i < end; i += step) {
        printf("%d ", i);
    }
    printf("\n");
}

int main() {
    int start, end, step;

    printf("Enter start value: ");
    scanf("%d", &start);
    printf("Enter end value: ");
    scanf("%d", &end);
    printf("Enter step value: ");
    scanf("%d", &step);

    range_gen(start, end, step);

    return 0;
}