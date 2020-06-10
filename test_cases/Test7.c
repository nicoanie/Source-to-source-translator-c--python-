#include <stdio.h>

int main(){
    int x;
    int y=10;
    int z=0;
    if(!((x==0&&(x>y||(z<0)))||(z==1)))
    {
        printf("%d", y);
    }
    return 0;
}