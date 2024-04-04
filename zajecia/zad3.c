#include <stdio.h>
#include <math.h>

int doskonala(int n){

    int suma=1;

    for (int i=2; i<=sqrt(n); i++){
        if (n%i==0)
        {
            suma += i;
            if (i!=n/i)
                suma += n/i;
        }
    }

    if (suma==n)
        return 1;
    return 0;
}

int main(void){
    int a, b, suma=0;
    printf("Enter a and b: ");
    scanf("%d%d", &a, &b);

    for (int i=a; i<=b; i++)
    {
        if (doskonala(i)==1)
        {
            printf("%d\n", i);
            suma+=1;
        }
    }
    
    printf("SUMA: %d", suma);
}