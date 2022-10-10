//Andrew McLaughlin

#include <stdlib.h>
#include <stdio.h>

#include "heap.h"  

#define DEBUG 1

int idxOfMaxValue(int* arr, int p, int left, int right, int N)
{
    int imv = p;

    if((left < N) && (arr[left] > arr[imv]))
    {
        imv = left;
    }

    if((right < N) && (arr[right] > arr[imv]))
    {
        imv = right;
    }

    return imv;
}

struct heap_struct make_heap_empty(int cap){
    struct heap_struct heap;

    int* arr = (int*) calloc(cap, sizeof(int));
    heap.items = arr;
    heap.N = 0;
    heap.capacity = cap;

    return heap;
}

struct heap_struct make_heap(int N, int * arr){
    struct heap_struct heap;
    heap.N = N;
    heap.capacity = N;
    heap.items = arr;

    printf(" in function make_heap, in DEBUG MODE, printing array BEFORE it gets turned into a heap :\n");
    print_heap(heap);

    int i;
    for(i = ((heap.N / 2) - 1); i >= 0; i--)
    {
        printf("in function make_heap, in DEBUG MODE, printing array after sink_down at index %d.\n",i);
        sink_down(i,heap.N,heap.items);
        print_heap(heap);
    }

    return heap;
}

void destroy(struct heap_struct * heapP){
    free(heapP->items);
    heapP->items = NULL;
    heapP->N = 0;
    heapP->capacity = 0;
}

void print_heap(struct heap_struct heapS){
    int i;

    printf("Heap:  size: %d, capacity: %d\n", heapS.N, heapS.capacity);

    printf("indexes:");
    for(i=0; i<heapS.N; i++) printf("%6d,", i);
    printf("\n");

    printf("values:");
    for(i=0; i<heapS.N; i++) printf("%6d,", heapS.items[i]);
    printf("\n\n");
}


void swim_up(int idx, int * arr){
    int temp;

    while((idx > 0) && (arr[idx] > arr[(idx - 1) / 2] ))
    {
        temp = arr[idx];
        arr[idx] = arr[(idx - 1) / 2];
        arr[(idx - 1) / 2] = temp;

        idx = (idx - 1) / 2;
    }
}


void sink_down(int i, int N, int * arr){ //i is current node, N is size of array
    int Left,Right,imv,temp;

    Left = (2 * i)+ 1;
    Right = (2 * i) + 2;
    imv = idxOfMaxValue(arr,i,Left,Right,N);
    
    while((imv != i) && (imv < N))
    {
        temp = arr[i];
        arr[i] = arr[imv];
        arr[imv] = temp;

        i = imv;
        Left = (2 * i) + 1;
        Right = (2 * i) + 2;
        imv = idxOfMaxValue(arr,i,Left,Right,N);
    }

}

void add(struct heap_struct * heapP, int new_item){ //done
    int capacity = heapP->capacity;

    if(heapP->N == heapP->capacity)
    {
        heapP->items = (int*) realloc((heapP->items), (2 * capacity) * sizeof(int));
        heapP->capacity = capacity * 2;
        heapP->items[heapP->N] = new_item;
        heapP->N = heapP->N + 1;
        swim_up((heapP->N - 1),heapP->items);

        printf("\nresizing\n");

    }
    else
    {
        heapP->items[heapP->N] = new_item;
        heapP->N = heapP->N + 1;
        swim_up((heapP->N - 1),heapP->items);
    }

}

int peek(struct heap_struct heapS){
    return heapS.items[0];
}


int poll(struct heap_struct * heapP){
    int max = heapP->items[0];
    heapP->items[0] = heapP->items[heapP->N - 1];

    heapP->N -= 1;
    sink_down(0, heapP->N, heapP->items);

    return max;
}