#include <stdio.h>
#include <stdlib.h>


#define MAX_LENGTH 120



// global variables ======================================================
int string_num; //saves the number of input sequences
char* dna[5]; //saves each string and its length
char* lcs; //stores the longest common subsequence

// helper functions ======================================================
void take_input(){
    FILE *input_file = fopen("hw2_input.txt", "r");

    //scan number of inputs
    fscanf(input_file, "%d", &string_num); 
    //remove unnecessary characters
    fscanf(input_file, "%*s"); 
    //scan the dna subsequences
    for(int i = 0; i < string_num; i++){
        dna[i] = (char*)malloc(sizeof(char) * MAX_LENGTH);
        fscanf(input_file, "%s", dna[i]);
    }
}

void print_final_results(){
    for (int i = 0; i < string_num; i++) {
        printf("%s\n", dna[i]);
    }
}

int main(){
    take_input();

    lcs = (char*)malloc(sizeof(char) * 1);
    lcs = (char*)realloc(lcs, sizeof(char) * 6);
    char lcs_str[7] = "ATCCAAT";
    for(int i = 0; i < 7; i++){
        lcs[i] = lcs_str[i];
    }

    print_final_results();

    //free memory
    for (int i = 0; i < string_num; i++) {
        free(dna[i]);
    }
    free(lcs);
}