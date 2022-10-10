//Andrew McLaughlin

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "heap.h"

int main() {
	int N,P,j,i=0;
    char fname[25];
    char line[128];
    char* token;
    int debug = 0;
    struct heap_struct heapS;
    printf("This program will create a max-heap and perform operations on it based on data from a file.\n\n");
    printf("Enter the filename: ");

    fgets(line,128,stdin);
    strtok(line,"\n");
    strcpy(fname,line);

    FILE* fp = fopen(fname,"r");
    fgets(line,128,fp);
    N = atoi(strtok(line,"\n"));

    //declaring new array
    int* arr = (int*) calloc(N, sizeof(int));

    // filling array
    fgets(line,128,fp);
    token = strtok(line," ");

    while(token != NULL)
    {
        arr[i] = atoi(token);
        token = strtok(NULL," ");

        i++;
    }

    //record number of operations
    fgets(line,128,fp);
    P = atoi(strtok(line,"\n"));

    //make heap
    heapS = make_heap(N,arr);
    print_heap(heapS);

    //interpret operations
    fgets(line,128,fp);
    token = strtok(line,"\n");
    token = strtok(token," ");

    for(j = 0; j < P; j++)
    {
        printf("Operation number %d, string: %s\n",j+1,token);

        if(token[0] == '*')
        {
            if(heapS.N == 0)
            {
                printf("Empty heap. no remove performed.removed:      0\n");
                print_heap(heapS);
            }
            else
            {
                printf("Removed     %d\n",poll(&heapS));
                print_heap(heapS);
            }
        }
        else if(!strcasecmp(token,"P"))
        {
            printf("peek:        %d\n",peek(heapS));
            print_heap(heapS);
        }
        else
        {
            printf("add:          %d\n",atoi(token));
            add(&heapS, atoi(token));
	        print_heap(heapS);
        }

        token = strtok(NULL, " ");

    }

    destroy(&heapS);
    fclose(fp);
}