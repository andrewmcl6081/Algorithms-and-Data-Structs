/*
compile:
gcc -g hw1_instructor.c hw1_student.c
run with valgrind:
valgrind --leak-check=full ./a.out
*/

#include "hw1_student.h"  // .h extenssion, not .c

// Functions that work with a dynamically allocated 2D array .
int** make_array_from_file(int* nrp, int* ncp);
void populate_array(int num_rows, int num_cols, int* arr2D[], FILE *fp);     
void free_array(int num_rows, int* arr2D[]);

// functions that work with a 1D array
void test_scores();
int test_scores_1(int thresh, int sz_in, int* arr_in, int sz_e_res, int* e_res);
void test_2D_array();


int main(){	
	test_scores();	
	test_2D_array();
}

// runs 3 hardcoded tests and prints failed or passed for each
void test_scores(){    
	printf("\n---------- test_scores - started -----------\n");
	int arr1[9] = {92, 57, 100, 95, 38, 10, 85, 91, 20};
	int res1[4] = {57, 38, 10, 20};
	
	int arr2[5] = {22, 45, 30, 49, 38};
	int res2[5] = {22, 45, 30, 49, 38};
		
	int passed_ct = 0;
	passed_ct += test_scores_1(60, 9, arr1, 4, res1);
	passed_ct += test_scores_1(50, 5, arr2, 5, res2);
	passed_ct += test_scores_1(20, 5, arr2, 0, NULL);
	
	printf ("\nPassed tests:  %d out of %d\n", passed_ct, 3);
	printf("\n---------- test_scores - finished -----------\n");
}


/* runs one test where arr_in is the input array, and it has size sz_in and
   e_res is the expected array and sz_e_res is the size of the expected result array
   It calls get_fail_grades() on arr_in and compares the content of the array returned by it
   with the content of the array e_res.
   It will free the dynamic array returned by get_fail_grades().
   Returns:
   1 - if passed given test
   0 - if falied given test 	
*/
int test_scores_1(int thresh, int sz_in, int* arr_in, int sz_e_res, int* e_res){
	int k, passed=0, size_res = -1;
	int * student_res;	// will store pointer to result array	
	
	student_res = get_scores_below(thresh, sz_in, arr_in, &size_res);	
	print_1D(size_res, student_res);
	if (size_res == sz_e_res) {
		passed = 1;
		for(k = 0; k<size_res; k++){
			if (student_res[k] != e_res[k] ) {
				passed = 0;
			}
		}
	}
	else {
		passed = 0; // student result did not find as many values as expected
	}
	
	if (passed==1) {
		printf("get_scores_below passed the sample test. \n");
	}
	else {
		printf("get_scores_below *** failed *** the sample test.\n");
	}	
		
	free (student_res);	
	
	return passed;
}




/*
============== 2D array functions ===================
*/

void test_2D_array(){
	printf("\n---------- test_2D_array - started -----------\n");
	int r, nr = 0, nc = 0, mn = -9999, mx = -9999;
	
	int** my_arr = make_array_from_file(&nr, &nc);  // my_arr = 1b1b;
	
	if (my_arr == NULL) return;
	
	print_2D(nr,nc, my_arr);
	
	update_min_max(nr,nc, my_arr, &mn, &mx);
	printf("min value: %d, max value: %d\n", mn, mx);
	
	free_array(nr, my_arr);	
	printf("\n---------- test_2D_array - finished -----------\n");
}


int** make_array_from_file(int* nrp, int* ncp){
	char fname[101]; // max length for file name? 99 + \0
	int num_rows, num_cols, r,c;
	FILE *fp;

	printf("Enter the filename: ");
	scanf("%s", fname);
	fp =fopen(fname, "r");
	if (fp == NULL){
		printf("File could not be opened.\n");
		return NULL;
	}
	fscanf(fp, "%d %d", &num_rows, &num_cols);	
	
	//int* arr2[num_rows];
	int** arr2;
	arr2 = calloc(num_rows, sizeof(int*) );	
	
	for(r = 0; r<num_rows; r++){
		arr2[r] = calloc( num_cols, sizeof(int) );
	}

	populate_array(num_rows, num_cols, arr2, fp);
	//print_array(num_rows, num_cols, arr2);
	
	(*nrp) = num_rows;
	(*ncp) = num_cols;
	
	fclose(fp);
	
	return arr2;
}

void free_array(int num_rows, int* arr2D[]){
	for(int r = 0; r<num_rows; r++){
		free( arr2D[r] );
	}
	free( arr2D );
}

void populate_array(int num_rows, int num_cols, int* arr2D[], FILE *fp){
	for (int r = 0; r<num_rows; r++){
		for(int c=0; c<num_cols; c++){
			fscanf(fp, "%d", &arr2D[r][c]);	
		}
	}	
}

