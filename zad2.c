#include <stdio.h>
#include <stdbool.h>


int main(void){
    int n, a=0, b=1;
    printf("Enter n: ");
    scanf("%d", &n);
    while(true)
    {
        if (n==a*b)
        {
            printf("Y");
            break;
        }
        else if (n<a*b)
        {
            printf("N");
            break;
        }
        int tmp = b;
        b = a+b;
        a = tmp;
    }
    return 0;
}