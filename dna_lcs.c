#include <stdio.h>
#include <stdlib.h>
#define MAX_LENGTH 120

// helper functions ======================================================
void take_input(char** dna, int* string_num){
    FILE *input_file = fopen("hw2_input.txt", "r");

    //scan number of inputs
    fscanf(input_file, "%d", string_num); 

    //remove unnecessary characters
    fscanf(input_file, "%*s"); 

    //scan the dna subsequences
    for(int i = 0; i < *string_num; i++){
        dna[i] = malloc(sizeof(char) * MAX_LENGTH);
        fscanf(input_file, "%s", dna[i]);
    }
    fclose(input_file);
}

void print_final_results(char** dna, int size){
    int lcs_index = 0;
    int max_length;
/*
    for(int i = 0; i < string_num; i++){
        if(len(dna[i]) > max_length){
            max_length = len(dna[i]);
        }
    }

    for(int j = 0; j < max_length; j++){
        
    }*/

    for (int i = 0; i < size; i++) {
        printf("%s\n", dna[i]);
    }
}

void find_lcs(char* lcs, int* lcs_length){

    char lcs_str[7] = "ATCCAAT";
    *lcs_length = 7;
    for(int i = 0; i < 7; i++){
        lcs[i] = lcs_str[i];
    }
}

int main(){
    int string_num = 0;
    char* dna[5];
    take_input(dna, &string_num); //store the input sequences

    char lcs[120];
    int lcs_length = 0;
    find_lcs(lcs, &lcs_length);

    print_final_results(dna, string_num);

    //free memory

}