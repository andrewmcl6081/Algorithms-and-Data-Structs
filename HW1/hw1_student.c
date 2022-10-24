#include "hw1_student.h"  // .h extenssion, not .c

// You do not need to change this print_1D function. It is included here for 
// your convenience, in case you need to call it while debugging.
void print_1D(int sz, int * arr){
	for(int k = 0; k< sz; k++){
		printf("%8d", arr[k]);
	}
	printf("\n");
}


/* dynamically allocates an array and populates it with all values from arr 
	that are strictly smaller than thresh. 
	Returns:
		- the pointer to the dynamically allocated array.
		- NULL if all elements of arr are greater or equal to thres. In this case it does not allocate any memory, and sets content of sz_res to 0.
*/
int* get_scores_below(int thresh, int sz_arr, int * arr, int* sz_res){
	int newSize = 0;

	for(int i = 0; i < sz_arr; i++)
	{
		if(arr[i] < thresh)
		{
			newSize++;
		}
	}

	if(newSize == 0)
	{
		*sz_res = 0;
		return NULL;
	}
	else
	{
        int* newArray = calloc(newSize, sizeof(int));
		int k = 0;

		for(int j = 0; j < sz_arr; j++)
		{
			if(arr[j] < thresh)
			{
				newArray[k] = arr[j];
				k++;
			}
		}

		if(newSize == 0)
		{
			*sz_res = 0;
			return NULL;
		}
		else
		{
			*sz_res = newSize;
			return newArray;
		}

		
	}

}

	


void update_min_max(int num_rows, int num_cols, int* arr2D[], int* arr_min, int* arr_max){

	int tempMax = 0;
	int tempMin = arr2D[0][0];

	for(int r = 0; r < num_rows; r++)
	{
		for(int c = 0; c < num_cols; c++)
		{
			if(arr2D[r][c] >= tempMax)
			{
				tempMax = arr2D[r][c];
			}

			if(arr2D[r][c] <= tempMin)
			{
				tempMin = arr2D[r][c];
			}
		}
	}

	*arr_min = tempMin;
	*arr_max = tempMax;
}


// improve this code to print as required in the homework
void print_2D(int num_rows, int num_cols, int* arr2D[]){	
	int r,c;

	printf("\n   |");
	for(int i = 0; i < num_cols; i++)
	{
		printf("%8d|",i);
	}

	printf("\n-------------------------------------------------");
	printf("\n");

	for (r = 0; r<num_rows; r++){
		printf("%3d|",r);

		for(c=0; c<num_cols; c++){
			printf("%8d|", arr2D[r][c]);
		}

		printf("\n");
	}	
}


