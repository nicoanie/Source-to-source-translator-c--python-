#include <stdio.h>
#include <stdbool.h>

float funzione(int x, float y, char z){

    int a = 4;
    float b = 6.35;

    b = a + b * 4;
    printf("%d", x);
    printf("%f", y);
    printf("%c", z);

    return b;
}

int main() {

    int x = 5;
    float y = 3.65;
    char z = 'g';

    x = funzione(y,z,x);
    printf("%d", x);

    return 0;
}