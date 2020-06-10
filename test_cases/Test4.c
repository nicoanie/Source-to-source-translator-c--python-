#include <stdio.h>


int func(int x){
    printf("Inserire un valore intero: ");
    scanf("%d", &x);
    printf("%d", x);
    return x;
}

int main(){
    int z;
    func (z);
    return 0;
}