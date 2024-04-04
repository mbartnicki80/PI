#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main(void){
    srand(time(NULL));
    int x = rand()%101;
    int n = rand()%101;
    int kroki=0;
    int a=0, b=101;
    printf("X = %d\n", x);
    while (n!=x)
    {
        if (n<x)
            a=n+1;
        else
            b=n;

        n=rand()%(b-a)+a;
        printf("%d\n", n);
        kroki++;
    }
    printf("Kroki: %d\n", kroki);
    return 0;
}