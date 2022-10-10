/* 
  Alexandra Stefan
 
 */
 
/* compile: 
gcc -g heap_calls.c heap.c
run:
./a.out 

Valgrind:
valgrind --leak-check=full ./a.out 
 * 
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "heap.h"  

int main()   {
	int N,k,i;
	char fname[501];
	int debug = 0;
	struct heap_struct heapS;
	printf("This program will call the heap functions.\n ");   
   

	N = 3;
	int *arr = (int*) calloc(N, sizeof(int) );
	arr[0] = 10;
	arr[1] = 20;
	arr[2] = 43;

	heapS = make_heap(N, arr);         
	print_heap(heapS);
	
	printf("removed: %6d\n", poll(&heapS) );	
	print_heap(heapS);

	printf("peek:    %6d\n", peek(heapS) );
	print_heap(heapS);

	printf("add:     %6d\n", 17);	
	add(&heapS, 17);
	print_heap(heapS);
	
	printf("removed: %6d\n", poll(&heapS) );	
	print_heap(heapS);

	printf("removed: %6d\n", poll(&heapS) );	
	print_heap(heapS);
   
	destroy(&heapS);   
	printf("After call to destroy (1)\n");
	print_heap(heapS);
   
	heapS = make_heap_empty(11);
	printf("Created empty heap: \n");
	print_heap(heapS);
	
	printf("add:     %6d\n", 204);	
	add(&heapS, 204);
	print_heap(heapS);	

	destroy(&heapS);   
	printf("After call to destroy(2)\n");
	print_heap(heapS);

	destroy(&heapS);   
	printf("After call to destroy(3)\n");
	print_heap(heapS);
	
	return 0;
}
