#include <stdio.h>
#include <math.h>
int tab[1000000];

int malej(int n){
    int ost = n%10;
    n/=10;
    while (n>0)
    {
        if (n%10>ost)
            return 0;
        ost=n%10;
        n/=10;
    }
    return 1;
}

void pierwsza(int n){

    tab[0]=1, tab[1]=1;

    for (int i=2; i<=sqrt(n); i++)
    {
        if (tab[i]==1)
            continue;
        for (int j=2*i; j<=n; j+=i)
                tab[j]=1;
    }
    return;
}

int main(void){
    int n;
    printf("Enter n: ");
    scanf("%d", &n);
    pierwsza(n);

    for (int i=2; i<n; i++)
        if (tab[i]==0)
            if (malej(i)==1)
                printf("%d\n", i);
    return 0;
}