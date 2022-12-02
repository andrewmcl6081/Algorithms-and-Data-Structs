#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "spell.h"

void display_table();
int calc_dist(int* prev,int* curr, char* str_vert,char* str_horiz);
int min(int a, int b);
int edit_distance(char * first_string, char * second_string, int print_table);
int compare(const void* a, const void* b);
char** populate_dict(char* filename);
void print_org_dict(char* filename, int size);
void print_sorted_dict(char** dict, int size);
void get_dict_size(char* filename, int* size);
int bin_word_search(char** dict,int size,char* target,int* count,int mode);
int is_char_seperator(char c);
int is_char_character(char c);
int minimum_dist(char** dictionary, int dict_size, char* word);
int find_num_of_smallest_dist_words(char** dict, int dict_size, int target, char* word);
void process_file(char** dict, int dict_size, char* input, char* output, int printOn);
void spell_check(char * testname, char * dictname, int printOn);

/*  Suggestions
- When you work with 2D arrays, be careful. Either manage the memory yourself, or
work with local 2D arrays. Note C99 allows parameters as array sizes as long as 
they are declared before the array in the parameter list. See: 
https://www.geeksforgeeks.org/pass-2d-array-parameter-c/

Worst case time complexity to compute the edit distance from T test words
 to D dictionary words where all words have length MAX_LEN:
Student answer:  Theta(DT * MAX_LEN^2)


Worst case to do an unsuccessful binary search in a dictionary with D words, when 
all dictionary words and the searched word have length MAX_LEN 
Student answer:  Theta(MAX_LEN * lg(D))
*/


/* You can write helper functions here */

int calc_dist(int* prev,int* curr, char* str_vert,char* str_horiz) {
	//prev[0] or curr[0] == row
	//prev[1] or curr[1] == col
	//prev[2] == value


	char vert_char = str_vert[curr[0] - 1];
	char horiz_char = str_horiz[curr[1] - 1];

	if( (prev[0] == curr[0] - 1) && (prev[1] == curr[1] - 1) ) {
		if(vert_char == horiz_char) {
			return prev[2];
		}
		else {
			return prev[2] + 1;
		}
	}
	else {
		return prev[2] + 1;
	}


}

int min(int a, int b) {
	return (a > b) ? b : a;
}

/*
Parameters:
first_string - pointer to the first string (displayed vertical in the table)
second_string - pointer to the second string (displayed horizontal in the table)
print_table - If 1, the table with the calculations for the edit distance will be printed.
              If 0 (or any value other than 1)it will not print the table
(See sample run)
Return:  the value of the edit distance (e.g. 3)
*/
int edit_distance(char * first_string, char * second_string, int print_table){
	int i,j;
	int left[3];
	int above[3];
	int diagonal[3];
	int num_rows = strlen(first_string) + 1;
	int num_cols = strlen(second_string) + 1;
	
	if(!strcmp(first_string,second_string)) {
		return 0;
	}
	else if(!strcmp(first_string,"0")) {
		return strlen(second_string);
	}
	else if(!strcmp(second_string,"0")) {
		return strlen(first_string);
	}


	int solutionTable[num_rows][num_cols];

	for(i = 0; i < num_rows; i++) {
		solutionTable[i][0] = i;
	}

	for(j = 0; j < num_cols; j++) {
		solutionTable[0][j] = j;
	}

	int curr_cordinate[2];

	for(i = 1; i < num_rows; i++) {
		for(j = 1; j < num_cols; j++) {
			left[0] = i;
			left[1] = j-1;
			left[2] = solutionTable[i][j-1];

			above[0] = i-1;
			above[1] = j;
			above[2] = solutionTable[i-1][j];

			diagonal[0] = i-1;
			diagonal[1] = j-1;
			diagonal[2] = solutionTable[i-1][j-1];

			curr_cordinate[0] = i;
			curr_cordinate[1] = j;

			int num_from_left = calc_dist(left,curr_cordinate,first_string,second_string);
			int num_from_above = calc_dist(above,curr_cordinate,first_string,second_string);
			int num_from_diagonal = calc_dist(diagonal,curr_cordinate,first_string,second_string);

			solutionTable[i][j] = min(min(num_from_left,num_from_above),num_from_diagonal);

			
		}
	}

	if(print_table == 1) {
		printf("\n");
		for(i = 0; i < (num_rows); i++) {
			for(j = 0; j < (num_cols); j++) {
				printf("  %d",solutionTable[i][j]);
			}

			printf("\n");
		}
		printf("\n");
	}

	return solutionTable[num_rows - 1][num_cols - 1];
}
	  

/*
Parameters:
testname - string containing the name of the file with the paragraph to spell check, includes .txt e.g. "text1.txt" 
dictname - name of file with dictionary words. Includes .txt, e.g. "dsmall.txt"
printOn - If 1 it will print EXTRA debugging/tracing information (in addition to what it prints when 0):
			 dictionary as read from the file	
			 dictionary AFTER sorting in lexicographical order
			 for every word searched in the dictionary using BINARY SEARCH shows all the "probe" words and their indices indices
			 See sample run.
	      If 0 (or anything other than 1) does not print the above information, but still prints the number of probes.
		     See sample run.
*/

int compare(const void* a, const void* b) {
	const char* str1_ptr = *(const char**) a;
	const char* str2_ptr = *(const char**) b;
	return strcmp(str1_ptr,str2_ptr);
}

char** populate_dict(char* filename) {
	int i;
	char line[128];
	char* token;
	FILE* fp = fopen(filename,"r");

	
	fgets(line,128,fp);
	token = strtok(line,"\n");
	int N = atoi(token);

	char** dictionary = (char**) malloc(N * sizeof(char*));

	for(i = 0; i < N; i++) {
		fgets(line,128,fp);
		token = strtok(line,"\n");
		
		dictionary[i] = (char*) malloc((strlen(token) + 1) * sizeof(char));
		strcpy(dictionary[i],token);
	}


	fclose(fp);
	return dictionary;
}

void print_org_dict(char* filename, int size) {
	int i;
	char line[128];
	char* modified_line;
	FILE* fp = fopen(filename,"r");

	fgets(line,128,fp); // throwing away first line containing size, already have it

	printf(" Original dictionary:\n");

	for(i = 0; i < size; i++) {
		fgets(line,128,fp);
		modified_line = strtok(line,"\n");

		printf("%d. %s\n",i,modified_line);
	}

	printf("\n");
	fclose(fp);
}

void print_sorted_dict(char** dict, int size) {
	int i;

	printf(" Sorted dictionary (alphabetical order) :\n");

	for(i = 0; i < size; i++) {
		printf("%d. %s\n",i,dict[i]);
	}

	printf("\n");
}

void get_dict_size(char* filename, int* size) {
	char line[128];
	char* token;
	FILE* fp = fopen(filename,"r");

	if(fp == NULL) exit(-1);

	fgets(line,128,fp);
	token = strtok(line,"\n");
	*size = atoi(token);

	fclose(fp);
}

int bin_word_search(char** dict,int size,char* target,int* count,int mode) {
	int left,right,mid;
	int i = 0;

	left = 0;
	right = size - 1;

	if(mode == 1) {
		printf("Binary search for: %s\n",target);
	}

	while( left <= right) {
		int m = (left + right) / 2;

		if(strcmp(target,dict[m]) == 0) {
			if(mode == 1) printf("dict[%d] = %s\n",m,dict[m]);

			i++;
			*count = i;

			return m;
		}

		if(strcmp(target,dict[m]) < 0) {
			if(mode == 1) printf("dict[%d] = %s\n",m,dict[m]);

			i++;
			right = m - 1;
		}
		else {
			if(mode == 1) printf("dict[%d] = %s\n",m,dict[m]);

			i++;
			left = m + 1;
		}
	}

	if(mode == 1) printf("Not found\n\n");
	*count = i;
	return -1;
}

int is_char_seperator(char c) {
	if((c != ' ') && (c != '.') && (c != ',') && (c != '!') && (c != '?') && (c != '\0')) {
		return 0;
	}
	else {
		return 1;
	}
}

int is_char_character(char c) {
	if((c != ' ') && (c != '.') && (c != ',') && (c != '!') && (c != '?')) {
		return 1;
	}
	else {
		return 0;
	}
	
}

int minimum_dist(char** dictionary, int dict_size, char* word) {
	int i = 0;
	int j = 0;
	int calculated_sizes[dict_size];

	for(i = 0; i < dict_size; i++) {
		calculated_sizes[i] = edit_distance(word,dictionary[i],0);
	}

	int min = calculated_sizes[0];

	for(j = 0; j < dict_size; j++) {
		if(calculated_sizes[j] < min) {
			min = calculated_sizes[j];
		}
	}


	return min;
}

int find_num_of_smallest_dist_words(char** dict, int dict_size, int target, char* word) {
	int counter = 0;
	int i = 0;

	for(i = 0; i < dict_size; i++) {
		if((edit_distance(word,dict[i],0)) == target) {
			counter++;
		}
	}

	return counter;
}

void process_file(char** dict, int dict_size, char* input, char* output, int printOn) {
	FILE* in = fopen(input,"r");

	if(in == NULL) exit(-1);

	FILE* out = fopen(output,"w");
	int i = 0;
	int j = 0;
	int count = 0;
	int result = 0;
	int user_answer = 0;
	int words_seen_in_Bs = 0;
	char seperator;
	int mode = 1;
	//1 == word wode, 2 == seperator mode

	char line[128];
	char line_answer[128];
	char* token;
	char word[50] = {'\0'};
	char low_word[50] = {'\0'};
	fgets(line,128,in);

	while(1 == 1) {

		if(mode == 1) {
			if(!(is_char_seperator(line[i]))) {
				word[count] = line[i];
				count++;
			}
			else {
				words_seen_in_Bs = 0;

				//making a lower case copy of our word
				for(j = 0 ; j < strlen(word); j++) low_word[j] = tolower(word[j]);

				result = bin_word_search(dict,dict_size,low_word,&words_seen_in_Bs,printOn);

				printf("\n---> |%s| (words compared when searching: %d)\n",word,words_seen_in_Bs);

				if(result != -1) {
					printf("     - OK\n\n");
				}
				else {

					int smallest_distance = minimum_dist(dict,dict_size,low_word);

					printf("-1 - type correction\n");
					printf(" 0 - leave word as is (do not fix spelling)\n");
					printf("     Minimum distance: %d (computed edit distances: %d)\n", smallest_distance, dict_size);
					printf("     Words that give minimum distance:\n");

					int num_smallest_dist = find_num_of_smallest_dist_words(dict,dict_size,smallest_distance,low_word);

					j = 0;
					char* potential_words[num_smallest_dist];
					for(int k = 0; k < dict_size; k++) {
						if((edit_distance(low_word, dict[k], 0)) == smallest_distance) {
							potential_words[j] = dict[k];
							printf(" %d - %s\n", j+1, potential_words[j]);
							j++;
						}
					}

					printf("Enter your choice (from the above options): ");
					fgets(line_answer,128,stdin);
					token = strtok(line_answer,"\n");
					user_answer = atoi(token);

					if(user_answer == -1) {
						printf("Enter correct word: ");
						fgets(line_answer,128,stdin);
						token = strtok(line_answer,"\n");
						strcpy(word,token);
					}
					else if(user_answer == 0) {

					}
					else {
						strcpy(word,potential_words[user_answer - 1]);
					}


				}

				fprintf(out,"%s",word);


				memset(low_word,0,50);
				memset(word,0,50);
				count = 0;
				mode = 2;
				
				if(line[i] == '\0') break;
			}
		}

		if(mode == 2) {
			seperator = line[i];
			
			fprintf(out,"%c",seperator);

			if(line[i+1] == '\0') break;
			if(is_char_character(line[i+1])) {
				mode = 1;
			}

		}


		i++;
	}

	fclose(in);
	fclose(out);
}

void spell_check(char * testname, char * dictname, int printOn){
	int dict_size;
	char out_file[20] = "out_";
	strcat(out_file,testname);
	get_dict_size(dictname,&dict_size);

	printf("Corrected output filename: %s\n\n",out_file);
	printf("Loading the dictionary file: %s\n\n",dictname);
	printf("Dictionary has size: %d",dict_size);

	if(printOn == 1) print_org_dict(dictname,dict_size);

	char** dictionary = populate_dict(dictname);
	qsort(dictionary,dict_size,sizeof(char*),compare);

	if(printOn == 1) print_sorted_dict(dictionary,dict_size);

	process_file(dictionary,dict_size,testname,out_file,printOn);
	
	for(int i = 0; i < dict_size; i++) {
		free(dictionary[i]);
	}

	free(dictionary);

	
}

