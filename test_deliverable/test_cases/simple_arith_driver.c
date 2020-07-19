#include <stdio.h>
int arith();

int main(){
        int k = arith();
        printf("%d", k);
        return !(22 == k );
}

