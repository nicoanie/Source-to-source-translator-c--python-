#include <stdio.h>

int main(){

    int a = 30;
    int c = 45;
    int x = 25;

    if(a > c){
        printf("%d", a);
    }else{
        if(a==c){
            printf("%d", x);
        }else{
            if(a<c){
                printf("%d", c);
            }
        }
    }
    return 0;
}
