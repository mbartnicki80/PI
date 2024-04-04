#include <stdio.h>

int main(void){
    unsigned long long n, silnia=1;
    printf("Enter n: ");
    scanf("%llu", &n);
    for (unsigned long long i=2; i<=n; i++)
        silnia *= i;

    printf("%llu\n", silnia);
    return 0;
}