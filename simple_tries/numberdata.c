#include <stdio.h>


int main()
{
    float a = 8.987654321;
    printf("The value of a = %0.9f", a);

    double b = 8.987654321;
    printf("\nThe value of b = %0.9lf", b);

    long double c = 8.987654321;
    printf("\nThe value of c = %0.9ld", c);
    return 0;
}