#include <stdio.h>
#include <string.h>

int main(void){

    char string[20];
    int k;
    printf("Enter string and k: ");
    scanf("%s%d", string, &k);
    int n = strlen(string);
    k%=26;
    for (int i=0; i<n; i++)
        if ((string[i]>='a')&&(string[i]<='z'))
            string[i] = (string[i]-'a'+k)%26+'a';
        else
            string[i] = (string[i]-'A'+k)%26+'A';

    printf("%s", string);

}