#include <stdio.h>
#include <stdbool.h>
int io(){
    char x = 'c';
    return x;
}

char b(){
    char x = 'd';
    return x;
}

float c(){
    float x = 3.65;
    return x;
}

bool d(){
    bool x = true;
    return x;
}

int main(){
    bool a;
    int x = 5;
    float y = 3.65;
    char z = 'b';
    bool w = true;

    a = io();
    printf("%d", a);
    a = b();
    printf("%d",a);
    a = c();
    printf("%d",a);
    a = d();
    printf("%d",a);
    a = x;
    printf("%d", a);
    a = y;
    printf("%d", a);
    a = z;
    printf("%d",a);
    a = w;
    printf("%d",a);
    return 0;
}
