/* 
 * Updated 2/17/2022 - Alexandra Stefan
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "list.h"


/* 
compile with -g to collect debugging info needed for Valgrind ( -lm links the math library): 
gcc -g bucket_sort.c list.c -lm

run with Valgrind:
valgrind --leak-check=full ./a.out

On some systems, but not all, you can also use the --show-leak-kinds=all flag:
valgrind --leak-check=full --show-leak-kinds=all ./a.out

run without Valgrind:
./a.out
*/

void print_array(int arr[], int N);

/* // recommended helper functions:
// function to insert a new node in a sorted list.
nodePT insert_sorted(nodePT L, nodePT newP);
//  function to sort an array sing bucket sort
void bucket_sort(int arr[], int N)
*/

void print_array(int arr[], int N){
	int j;
	printf("\n array: ");
	for(j= 0; j<N; j++){
		printf("%5d,", arr[j]);
	}
	printf("\n");
}

//-------------------------------------------------------------

void print_list_horiz(struct node * my_list) {
    if (my_list == NULL) {
		printf("\n<print_list_horiz> : List is NULL\n");
        return;
    }
    int i = 0;
    struct node * curr;

    printf("\n List items: ");
    for (i = 0, curr = my_list; (curr != NULL); curr = curr->next) {
        printf("%5d    ", curr->data);
        i++;
    }
	printf("\n Length of above list = %d\n", i);
}

void destroy_list(nodePT L) {
    nodePT next;
	nodePT curr = L;
	L = NULL;   // can remove this line, not needed
    while (curr != NULL) {
        next = curr->next;
        free(curr);
        curr = next;
    }
}

int populate_array(int** arr,int* size)
{
	int i;
	char fileName[20];
	FILE* fp = NULL;

	printf("\nEnter the filename: ");
	scanf("%s",fileName);
	fp = fopen(fileName,"r");

	if(fp == NULL)
	{
		printf("File could not be opened.\n");
		return 0;
	}
	else
	{
		fscanf(fp,"%d",size);
		*arr = malloc((*size) * sizeof(int));


		for(i=0;i<*size;i++)
		{
			fscanf(fp,"%d",&((*arr)[i]));
		}

		fclose(fp);
		return 1;
	}
}

struct node** create_array_of_lists(int size)
{
	struct node** arr = (struct node**)malloc(size * sizeof(struct node*));
	return arr;
}

void find_max_min(int* max,int* min,int* arr,int size)
{
	int i;
	for(i=0; i<size; i++)
	{
		if(arr[i] < *min)
		{
			*min = arr[i];
		}

		if(arr[i] > *max)
		{
			*max = arr[i];
		}
	}
}

void insert_node_into_list(struct node* newNode,struct node** listArray,int idx)
{
	struct node* head = listArray[idx];
	struct node* current = head;

	// no link list present in index
	if(head == NULL)
	{
		listArray[idx] = newNode;
	}
	else if(newNode->data < head->data)
	{
		//test to see if we need to make a new head of the list
		newNode->next = head;
		listArray[idx] = newNode;
	}
	else
	{
		while(current->next != NULL && newNode->data > current->next->data)
		{
			current = current->next;

		}

		newNode->next = current->next;
		current->next = newNode;
	}

}

struct node* make_node(int element)
{
	struct node* newNode = (struct node*)malloc(sizeof(struct node));
	newNode->data = element;
	newNode->next = NULL;

	return newNode;
}

void sort(int* currArray,struct node** listArray,int size,int min,int max)
{
	int i,j;
	int idx;
	double Min,Max,arrayValue;

	printf("\nBucketsort: min=%d, max = %d, N=%d buckets\n",min,max,size);

	for(i=0;i<size;i++)
	{
		listArray[i] = NULL;
	}

	for(j=0;j<size;j++)
	{
		//type casting to long
		Min = min;
		Max = max;
		arrayValue = currArray[j];

		idx = floor(((arrayValue - Min) * size)/ ((1 + Max) - Min));
		printf("arr[%d]=%5d, idx = %d\n",j,currArray[j],idx);
		
		struct node* newNode = make_node(currArray[j]);
		insert_node_into_list(newNode,listArray,idx);
	}
}

void print_out(struct node** listArray,int size)
{
	int i;
	for(i=0;i<size;i++)
	{
		printf("------ List at index %d :",i);
		print_list_horiz(listArray[i]);
	}
}

void insert_back_to_array(struct node** listArray,int* arr,int size)
{
	struct node* head;
	struct node* current;

	int array_idx = 0;
	int list_idx = 0;

	while(list_idx < size)
	{
		if(listArray[list_idx] == NULL)
		{
			list_idx++;
			destroy_list(listArray[list_idx - 1]);
		}
		else
		{
			current = listArray[list_idx];
			while(current != NULL)
			{
				arr[array_idx] = current->data;
				current = current->next;

				array_idx++;
			}

			list_idx++;
			destroy_list(listArray[list_idx - 1]);
		}
	}

	free(listArray);
}

void bucket_sort(int* arr,int size)
{
	int min = arr[0];
	int max = 0;
	
	find_max_min(&max,&min,arr,size);

	struct node** listArray = create_array_of_lists(size);

	sort(arr,listArray,size,min,max);
	
	print_out(listArray,size);

	insert_back_to_array(listArray,arr,size);
	
}

void run1(){
	int* inputArray = NULL;
	int size;

	if(!populate_array(&inputArray,&size))
	{
		return;
	}

	print_array(inputArray,size);

	bucket_sort(inputArray,size);

	print_array(inputArray,size);

	free(inputArray);

	

}

int main()
{
	printf("This program will read a file name, load data for an array from there and print the sorted array.\n");
	printf("The array is sorted using bucket sort.\n");
	printf("This will be repeated as long as the user wants.\n");
	int option;
	do {
		run1();
		printf("\nDo you want to repeat? Enter 1 to repeat, or 0 to stop) ");
		char ch;
		scanf("%d%c",&option, &ch);  // ch is used to remove the Enter from the input buffer
 	} while (option == 1);

   return 0;
}
