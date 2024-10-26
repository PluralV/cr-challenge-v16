#ifndef XOR_PRIMES_H
#define XOR_PRIMES_H
#include <stdio.h>
#include <math.h>
#include <stdlib.h>



typedef struct list_node{
    int data;
    struct list_node *next;
}list_node;

unsigned long xor_product(unsigned int xi, unsigned int yi);
list_node *divisors_of(int x);
int *mobius_2_sieve(int n);
long count_irreducible(unsigned int n);
long sieve(int n);

void print_list_nodes(list_node *lst){
    while (lst != NULL){
        fprintf(stderr,"[%d]",lst->data);
        if (lst->next != NULL){
            fprintf(stderr," -> ");
            lst = lst->next;
        } else{
            fprintf(stderr,"\n");
            break;
        } 
    }
}
#endif