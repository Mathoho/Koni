#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void chpont(char *arr) {
 //arr[4] = 'k';
 *(arr + 4) ='z';   
}

int main(int argc, char *argv) {
    char arr[] = {'a','b','c','d','e','f','g','h'};
    printf("the char is before %c\n",arr[4]);
    chpont(arr);
    printf("the char is after %c\n",arr[4]);
}