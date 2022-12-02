//Andrew McLaughlin
//1001988355

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int** make_matrix(char* filename, char** courses, int numOfCourses);
void DFS(int** matrix, int* top_sort_result, int* idx_ptr, int numOfCourses, char** courses);
void DFS_visit(int** matrix, int* top_sort_result, int* idx_ptr, int numOfCourses,int u,char** color,int* pred,char** courses);
void print_matrix(int numOfCourses, int** adjacency_matrix);
void reverse_array(int* top_array, int numOfCourses);
void free_memory(int** matrix, char** courses, int* top_sort_result, int* pred, char** color, int numOfCourses);

int main(int argc, char** argv) {
    char filename[30];
    char line[1000];
    char* token;
    FILE* fp;
    int numOfCourses = 0;


    printf("This program will read, from a file, a list of courses and their prerequisites and will print the list in which to take courses.\n");

    printf("Enter filename: ");
    fgets(line,1000,stdin);
    token = strtok(line,"\n");
    strcpy(filename,token);

    fp = fopen(filename,"r");

    if(fp == NULL || strlen(filename) > 30) {
        printf("Could not open file %s. Exit\n",filename);
        printf("\nFailed to read from file. Program will terminate.\n");

        return -1;
    }

    while(fgets(line,1000,fp) && (line[0] != '\n')) {
        numOfCourses ++;
    }
    fclose(fp);

    printf("Number of vertices in built graph:  N = %d\n",numOfCourses);

    fp = fopen(filename,"r");
    char** courses = (char**) malloc(sizeof(char*) * numOfCourses);

    for(int i = 0; i < numOfCourses; i++) {
        fgets(line,1000,fp);
        token = strtok(line," \n");

        courses[i] = (char*) malloc(sizeof(char) * 30);
        strcpy(courses[i],token);
    }

    fclose(fp);

    printf("Vertex - coursename correspondence\n");
    for(int i = 0; i < numOfCourses; i++) {
        printf("%d - %s\n",i,courses[i]);
    }
    printf("\n");


    int** adjacency_matrix = make_matrix(filename,courses,numOfCourses);
    int* top_sort_result = (int*) malloc(sizeof(int) * numOfCourses);
    int result_index = 0;
    int* index_ptr = &result_index;

    print_matrix(numOfCourses,adjacency_matrix);

    DFS(adjacency_matrix,top_sort_result,index_ptr,numOfCourses,courses);

    reverse_array(top_sort_result,numOfCourses);

    printf("Order in which to take courses:\n");

    for(int i = 0; i < numOfCourses; i++) {
        printf("%d. - %s (corresponds to graph vertex %d)\n",(i+1),courses[top_sort_result[i]],top_sort_result[i]);
    }

    // freeing all memory
    
    free(top_sort_result);
    for(int r = 0; r < numOfCourses; r++) {
        free(adjacency_matrix[r]);
    }
    free(adjacency_matrix);

    for(int i = 0; i < numOfCourses; i++) {
        free(courses[i]);
    }
    free(courses);

    return 0;
}

void DFS(int** matrix, int* top_sort_result, int* idx_ptr, int numOfCourses, char** courses) {
    char** color = (char**) malloc(sizeof(char*) * numOfCourses);
    int* pred = (int*) malloc(sizeof(int) * numOfCourses);

    for(int u = 0; u < numOfCourses; u++) {
        color[u] = (char*) malloc(sizeof(char) * 6);
        strcpy(color[u],"WHITE");
    }

    for(int u = 0; u < numOfCourses; u++) {
        pred[u] = -1;
    }

    //start search

    for(int u = 0; u < numOfCourses; u++) {
        if(!strcmp(color[u],"WHITE")) {
            DFS_visit(matrix,top_sort_result,idx_ptr,numOfCourses,u,color,pred,courses);
        }
    }

    // freeing all memory

    free(pred);
    for(int i = 0; i < numOfCourses; i++) {
        free(color[i]);
    }
    free(color);
}

void DFS_visit(int** matrix, int* top_sort_result, int* idx_ptr, int numOfCourses,int u,char** color,int* pred,char** courses) {
    // u is the current vertex we are at
    strcpy(color[u],"GRAY");

    // v is vertices that are adjacent to u
    for(int v = 0; v < numOfCourses; v++) {
        if(!strcmp(color[v],"WHITE") && (matrix[u][v] == 1)) {
            pred[v] = u;
            DFS_visit(matrix,top_sort_result,idx_ptr,numOfCourses,v,color,pred,courses);
        }

        if(!strcmp(color[v],"GRAY") && (v != u) && (matrix[u][v] != 0)) {
            printf("There was at least one cycle. There is no possible ordering of the courses.\n");
            free_memory(matrix,courses,top_sort_result,pred,color,numOfCourses);
            exit(0);
        }
    }

    strcpy(color[u],"BLACK");
    top_sort_result[*idx_ptr] = u;
    *idx_ptr = *idx_ptr + 1;
}

int** make_matrix(char* filename, char** courses, int numOfCourses) {
    char line[1000];
    char* token;
    int source = 0;
    int destination = 0;
    int** matrix = (int**) malloc(sizeof(int*) * numOfCourses);

    for(int r = 0; r < numOfCourses; r++) {
        matrix[r] = (int*) malloc(sizeof(int) * numOfCourses);
    }

    for(int r = 0; r < numOfCourses; r++) {
        for(int c = 0; c < numOfCourses; c++) {
            matrix[r][c] = 0;
        }
    }

    FILE* fp = fopen(filename,"r");

    //go through each line in the file
    for(int i = 0; i < numOfCourses; i++) {
        fgets(line,1000,fp);
        //parse for first course in line
        token = strtok(line, " \n");
        
        for(int i = 0; i < numOfCourses; i++) {
            //find index of destination vertices
            if(!strcmp(token,courses[i])) {
                destination = i;
                break;
            }
        }
        
        token = strtok(NULL," \n");

        while(token != NULL) {
            for(int i = 0; i < numOfCourses; i++) {
                if(!strcmp(token,courses[i])) {
                    source = i;
                    break;
                }
            }

            matrix[source][destination] = 1;
            
            token = strtok(NULL," \n");
        }
    }

    fclose(fp);
    return matrix;
}

void print_matrix(int numOfCourses, int** adjacency_matrix) {

    printf("Adjacency matrix:\n");
    printf("    |");
    for(int i = 0; i < numOfCourses; i++) {
        printf("%4d",i);
    }
    printf("\n");
    printf("-----");
    for(int i = 0; i < numOfCourses; i++) {
        printf("----");
    }
    printf("\n");
    for(int r = 0; r < numOfCourses; r++) {
        printf("%4d|",r);
        for(int c = 0; c < numOfCourses; c++) {
            printf("%4d",adjacency_matrix[r][c]);
        }
        printf("\n");
    }
    printf("\n\n");
}

void reverse_array(int* top_array, int numOfCourses) {
    int start = 0;
    int end = numOfCourses - 1;
    int temp;

    while(start < end) {
        temp = top_array[start];
        top_array[start] = top_array[end];
        top_array[end] = temp;

        start++;
        end--;
    }
}

void free_memory(int** matrix, char** courses, int* top_sort_result, int* pred, char** color, int numOfCourses) {

    for(int i = 0; i < numOfCourses; i++) {
        free(matrix[i]);
    }
    free(matrix);

    for(int i = 0; i < numOfCourses; i++) {
        free(courses[i]);
    }
    free(courses);

    free(top_sort_result);

    free(pred);

    for(int i = 0; i < numOfCourses; i++) {
        free(color[i]);
    }
    free(color);
}
