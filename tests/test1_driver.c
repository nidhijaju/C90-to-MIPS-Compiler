#include <stdio.h>
int hello();

int main(){
        int k = hello();
        printf("%d", k);
    return !(1 == hello());
}