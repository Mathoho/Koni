
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "heap.h"
#include "huffman.h"

void heap_initialize(Heap *H)
{
    H->n = 0;
    int i;

    for (i = 0; i < MAX_HEAP_SIZE; i++) {    
        HeapNode temp;
        temp.frequency = 0;
        temp.c = '\0';
        temp.left = NULL;
        temp.right = NULL;
        H->a[i] = temp;
    }

}

void heap_insert(Heap *H, HeapNode *node)
{
    //first line
    H->a[H->n] = *node;
    int c = H->n;
    int p = (c -1)/2;
    while (p >= 0) {
        if (strcmp(H->a[p].c, H->a[c].c) > 0) {
            //swap
            HeapNode temp = H->a[p];
            H->a[p] = H->a[c];
            H->a[c] = temp;

            c = p;
            p = (c-1)/2;
        } else {
            p = -1;
            //p = p -1;
        }   
    }
    H->n = H->n +1;
}
void heap_remove(Heap *H, HeapNode *node)
 {
    if(H->n >= 0){
        HeapNode min = H->a[0];
        H->a[0] = H->a[H.n-1];
        H->n = H->n - 1;
        int p = 0;
        int c = 2*p +1;
        while (c <= (H->n - 1)){
            if (H->a[c].frequency >= H->a[c+1].frequency) {
                c = c + 1;
            }
            if (H->a[c].frequency <= H->a[p].frequency) {
                HeapNode temp = H->a[c];
                H->a[c] = H->a[p];
                H->a[p] = temp;
                //also check the xchng thing but i think you cant because the registers are 4bytes
                //but check it out
                p = c;
                c = 2*p + 1;
            } else {
                c = H->n;
            }
        }
        //make sure that when you return the value you are llooking is in eax
}
void huffman_build_tree(Heap *H, HeapNode **t){
    //insert using a string which i dont know where it is
    //i think we have to do the bin to string thing to get the string 
    while (H->n > 0)      /* H != NULL*/
    {

        if(H->n ==1) {
            //remove(h,x);
        }
        //remove x,y
        //make a new heap node 
        //since we are making it in assembly make sure when you return you have the vlue of x in eax
        //have the value if y in eax

    }
    

}
