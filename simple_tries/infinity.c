#include <stdio.h>


void infinity() {
    while (1) {
        printf("This will run forever...\n");
    }
}

int main() {
    printf("Starting infinite loop...\n");
    infinity();
    return 0; // This line will never be reached
}

