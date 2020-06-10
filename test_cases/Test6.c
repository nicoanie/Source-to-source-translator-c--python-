#include <stdio.h>
#include <stdbool.h>
int ab( int a, float b, char c){
    printf("%d", a);
    printf("%f", b);
    printf("%c", c);

    return a;
}
int main(){
    int a= 10;
    float b = 3.56;
    char c = 'd';

    ab(b, c, a);

    return 0;
}