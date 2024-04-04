#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

int main(void){
    srand(time(NULL));
    double a, b;
    int n;
    printf("Enter a, b, n: ");
    scanf("%lf%lf%d", &a, &b, &n);
    
    printf("%d", n);
    int pole=abs(b-a)*2, c=0;
    
    for (int i=0; i<n; i++)
    {
        double x=rand()%abs(b-a)+a;
        double y=rand()%3-1;
        if (y<=sin(x) && y>0)
            c++;
        else if (0>y && y>=sin(x))
            c--;
    }
    double calka = pole*c/n;
    printf("%lf", calka);
    return 0;
}