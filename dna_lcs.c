#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

// returns 1 if the values are the same at the given index
int is_common(char** dna, int size, char* lcs, int lcs_index, int indexes[5]){
    for(int i = 0; i < size; i++){
        if(dna[i][indexes[i]] != lcs[lcs_index]){
            return 0;
        }
    }
    return 1;
}

void write_final_results(char** dna, int size, char* lcs){
    int lcs_index = 0;
    char* aligned_dna[size];
    int max_length = 0;
    int indexes[5] = {0};

    for(int i = 0; i < size; i++){
        aligned_dna[i] = malloc(sizeof(char) * 500);
        for (int j = 0; j < 500; j++) {
            aligned_dna[i][j] = '-';
        }
    }
    
    FILE *output;
    output = fopen("hw2_output.txt", "w");

    int aligned_index = 0;
    int count = 0; //count the number of strings that are longer than the lcs
    while(count < size){    
        for(int i = 0; i < size; i++){
            if(indexes[i] > strlen(dna[i])){ //if longer than the string, continue
                count++;
            } 
            // if dna sequence is longer than lcs, just append the rest 
            else if(lcs_index > strlen(lcs)){
                aligned_dna[i][aligned_index] = dna[i][indexes[i]];
            }
            //if current character is different from lcs, add it to aligned dna
            else if(dna[i][indexes[i]] != lcs[lcs_index]){
                aligned_dna[i][aligned_index] = dna[i][indexes[i]];
            } 
            // if current character is the same as lcs and is common, add it to aligned dna
            else if(dna[i][indexes[i]] == lcs[lcs_index] && is_common(dna, size, lcs, lcs_index, indexes)){
                aligned_dna[i][aligned_index] = dna[i][indexes[i]];
                lcs_index++;
            }
            // if current character is the same as lcs, but just not in the right place, leave it as a dash
            else{
                indexes[i]--;
            }
        }

        aligned_index++;

        for(int i = 0; i< size; i++){
            indexes[i]++;
        }
    }

    //write aligned sequences to file
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < aligned_index-size+1; j++){
            fputc(aligned_dna[i][j], output);
        }
        fputc('\n', output);
        printf("%s\n", aligned_dna[i]);
    }

    //print asterisks
    int i = 0;
    for(int j = 0; j < aligned_index-size+1; j++){
        if(i < strlen(lcs) && lcs[i] == aligned_dna[0][j]){
            fputc('*', output);
            i++;
        }else{
            fputc(' ', output);
        }
    }

    fclose(output);
}

void find_lcs(char* lcs, int* lcs_length){
    char lcs_str[6] = "ATCCAT";
    *lcs_length = 6;
    for(int i = 0; i < 6; i++){
        lcs[i] = lcs_str[i];
    }
}

int main(){
    int string_num = 0;
    char* dna[5];
    take_input(dna, &string_num); //store the input sequences

    int* matrix;
    char lcs[120];
    int lcs_length = 0;
    find_lcs(lcs, &lcs_length);

    write_final_results(dna, string_num, lcs);

    //free memory
    for(int i = 0; i < string_num; i++){
        free(dna[i]);
    }
    //free(lcs);
}