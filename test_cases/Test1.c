#include <stdio.h>
#include <stdbool.h>

int a;
int d;

int main() {
    int a = 5;
    int b = 4;
    int c;
    float x = 3.5;
    char y = 'z';
    bool z;

    while (a>0){
        if(a == 3){
            printf("%f", x);
            a = a-1;
            z=true;
        }
        else{
            printf("%c", y);
            a = a-1;
            z=false;
        }
        printf("%d", z);
    }
    printf("Inserire variabile intera: ");
    scanf("%d", &c);
    c = a+b*4-c;
    printf("%d", c);

    return 0;
} 