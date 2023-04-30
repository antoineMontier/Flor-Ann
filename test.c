#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void swap(int* add_a, int* add_b){
    int tmp = *add_a;
    *add_a = *add_b;
    *add_b = tmp;
    add_a = 0x7fffa1fb0000;
}

int addition(int a, int b){return a+b;}


int main(){
    int a = 5;
    int b = -1;

    int* adresse_de_a = &a;
    int* adresse_de_b = &b;

    printf("a = %d, b = %d, addr a = %p , addr b = %p\n", a, b, adresse_de_a, adresse_de_b);

    swap(adresse_de_a, adresse_de_b);

    printf("a = %d, b = %d, addr a = %p , addr b = %p\n", a, b, adresse_de_a, adresse_de_b);

    printf("%d\n", addition(5, -1));


    return 0;
}