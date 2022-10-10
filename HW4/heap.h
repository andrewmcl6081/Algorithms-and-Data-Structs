
#ifndef HEAP_H
#define	HEAP_H

struct heap_struct {
	int* items;
	int N;  // current size
	int capacity; // array capacity
};

// max-heap operations

struct heap_struct make_heap_empty(int cap);

// assumes arr was dynamically allocated. 
struct heap_struct make_heap(int N, int * arr); // makes a max-heap from arr. Asssumes both size and capacity are N.

// Will free the heap array.
void destroy(struct heap_struct * heapP);

void print_heap(struct heap_struct heapS);

void sink_down(int i, int N, int * arr);
void swim_up(int idx, int * arr);

int peek(struct heap_struct heapS);
int poll(struct heap_struct * heapP);
void add(struct heap_struct * heapP, int new_item);// will resize the heap if needed


#endif	/* HEAP_H */
