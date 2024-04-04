#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

int k = 0;

int rand_from_interval(int a, int b) {

        if (a>b)
            return INT_MIN;
        if ((b-a)>RAND_MAX)
            return INT_MAX;
        if (a==b)
            return a;
        return rand()%(b-a+1)+a;
}

void rand_permutation(int n, int tab[]) {
    if (n<0)
        return;
    for (int i=0; i<n; i++)
        tab[i] = i;
    for (int i=0; i<n-1; i++)
    {
        int k=rand_from_interval(i, n-1);
        int tmp = tab[i];
        tab[i] = tab[k];
        tab[k] = tmp;
    }
}

#define CBUFF_SIZE 52
int cbuff1[CBUFF_SIZE], cbuff2[CBUFF_SIZE];
int out1 = 0, len1 = 0, out2 = 0, len2 = 0;

int cbuff_push(int cli_nr, int p) {
    if (p==1) {
        if (len1>=CBUFF_SIZE)
        return -2;

        cbuff1[(len1+out1)%CBUFF_SIZE] = cli_nr;
        len1++;
        return 0;
    }
    else if (p==2) {
        if (len2>=CBUFF_SIZE)
        return -2;

        cbuff2[(len2+out2)%CBUFF_SIZE] = cli_nr;
        len2++;
        return 0;
    }
}

int cbuff_pop(int p) {
    if (p==1) {
        if (len1==0)
            return -1;
        int a = out1;
        out1 = (out1+1)%CBUFF_SIZE;
        len1--;
        return cbuff1[a];
    }
    else if (p==2) {
        if (len2==0)
            return -1;
        int a = out2;
        out2 = (out2+1)%CBUFF_SIZE;
        len2--;
        return cbuff2[a];
    }
}

int cbuff_state(int p) {
    if (p==1)
	    return len1;
    else if (p==2)
        return len2;
}

void cbuff_print(int p) {
    if (p==1)
        for (int i=0; i<len1; i++)
                printf("%d ", cbuff1[(out1+i)%CBUFF_SIZE]);
    else if (p==2)
        for (int i=0; i<len2; i++)
                printf("%d ", cbuff2[(out2+i)%CBUFF_SIZE]);
}

void tasowanie(int cbuff1[], int cbuff2[]) {
    int tab[CBUFF_SIZE];
    rand_permutation(CBUFF_SIZE, tab);

    for (int i=0; i<CBUFF_SIZE/2; i++)
        cbuff_push(tab[i], 1);
    for (int i=CBUFF_SIZE/2; i<CBUFF_SIZE; i++)
        cbuff_push(tab[i], 2);

}

int war(int a, int b) {
    int cbuff1_tmp[CBUFF_SIZE];
    int cbuff2_tmp[CBUFF_SIZE];
    cbuff1_tmp[0] = a;
    cbuff2_tmp[0] = b;
    k++;
    for (int i=1; i<CBUFF_SIZE; i++) {
        if (cbuff_state(1)==0 || cbuff_state(2)==0) {
            for (int j=0; j<i; j++)
                cbuff_push(cbuff1_tmp[j], 1);
            for (int j=0; j<i; j++)
                cbuff_push(cbuff2_tmp[j], 2);
            return 1;
        }
        cbuff1_tmp[i] = cbuff_pop(1);
        cbuff2_tmp[i] = cbuff_pop(2);
        if (i%2==0) {
            if (cbuff1_tmp[i]/4>cbuff2_tmp[i]/4) {
                for (int j=0; j<=i; j++)
                    cbuff_push(cbuff1_tmp[j], 1);
                for (int j=0; j<=i; j++)
                    cbuff_push(cbuff2_tmp[j], 1);
                k++;
                return 0;
           }
            else if (cbuff1_tmp[i]/4<cbuff2_tmp[i]/4) {
                for (int j=0; j<=i; j++)
                    cbuff_push(cbuff2_tmp[j], 2);
                for (int j=0; j<=i; j++)
                    cbuff_push(cbuff1_tmp[j], 2);
                k++;
                return 0;
           }
            else
                k+=2;
        }
    }
    return 0;
}

int konflikt(int wersja) {
    int a = cbuff_pop(1);
    int b = cbuff_pop(2);
    int wyn = 0;
    if ((a/4)>(b/4)) {
        cbuff_push(a, 1);
        cbuff_push(b, 1);
        k++;
    }
    else if ((b/4)>(a/4)) {
        cbuff_push(b, 2);
        cbuff_push(a, 2);
        k++;
    }
    else {
        if (wersja==1) {
            cbuff_push(a, 1);
            cbuff_push(b, 2);
            k++;
        }
        else
            wyn = war(a, b);
    }
    return wyn;
}

int main(void) {

    int seed, wersja, maxk;
    scanf("%d%d%d", &seed, &wersja, &maxk);
    srand(seed);
    tasowanie(cbuff1, cbuff2);
    int a;
    while (cbuff_state(1)>0 && cbuff_state(2)>0 && k<maxk) {
        a = konflikt(wersja);
        if (a==1) {
            printf("1 %d %d", cbuff_state(1), cbuff_state(2));
            return 0;
        }
    }
    if (k==maxk)
        printf("0 %d %d", cbuff_state(1), cbuff_state(2));
    else if (cbuff_state(2)==0)
        printf("2 %d", k);
    else {
        printf("3\n");
        cbuff_print(2);
    }
    return 0;
}
