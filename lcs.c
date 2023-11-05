#include <stdio.h>
#include <stdlib.h>

#define MAX_LENGTH 120

typedef struct DNA_STRING_INFO{
    char* dna_str;
    int size;
} DNA;

DNA dna[5]; //saves each string and its length
int string_num; //saves the number of input strings
char* lcs; //stores the longest common subsequence

//helper functions ======================================================
int is_common(int arr_index, int lcs_index){
    char c = lcs[lcs_index];

    for(int i = 0; i < string_num; i++){
        if(dna[i].dna_str[arr_index] != c) return 0; 
    }

    return 1;
}

void print_final_results(){
    int max_str = 120;
    char* asterisks[max_str];
    int lcs_index = 0;

    for(int j = 0; j < max_str; j ++){
        if(is_common(j, lcs_index)){
                asterisks[j] = "*";
                lcs_index++;
        }
    }

    //print results
    for (int i = 0; i < string_num; i++) {
        printf("%s\n", dna[i].dna_str);
    }
    //printf("%s", asterisks);
}

//main ==================================================================
int main(){
    FILE *input_file = fopen("hw2_input.txt", "r");
    
    if(input_file == NULL) return 0; // if files doesn't exist, return

    //scan number of inputs
    fscanf(input_file, "%d", &string_num); 
    //remove unnecessary characters
    fscanf(input_file, "%*s"); 

    for(int i = 0; i < string_num; i++){
        int j = 0;
        dna[i].dna_str = (char*)malloc(sizeof(char) * MAX_LENGTH);
        fscanf(input_file, "%s", dna[i].dna_str);
    }

    lcs = (char*)malloc(sizeof(char) * 1);
    lcs = (char*)realloc(lcs, sizeof(char) * 6);
    char lcs_str[7] = "ATCCAAT";
    for(int i = 0; i < 7; i++){
        lcs[i] = lcs_str[i];
    }

    print_final_results();

    //free memory
    for (int i = 0; i < string_num; i++) {
        free(dna[i].dna_str);
    }
    free(lcs);
}