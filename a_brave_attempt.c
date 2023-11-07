#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LENGTH 120
//struct
typedef struct dna_seq{
    char* sequence;
    int length;
    int* A;
    int* T;
    int* G;
    int* C;
} DNA_SEQ;

typedef struct node{
    int str1_index;
    int str2_index;
    int str3_index;
    int str4_index;
    int str5_index;
    struct NODE *A;
    struct NODE *T;
    struct NODE *G;
    struct NODE *C;
    int lcs_length;
}NODE;

typedef struct memo{
    int str1_index;
    int str2_index;
    int str3_index;
    int str4_index;
    int str5_index;
    NODE* return_address;
} MEMO;



// helper functions ============================================================
// takes input
void take_input(DNA_SEQ** dna, int* string_num){
    FILE *input_file = fopen("hw2_input.txt", "r");
    //FILE *input_file = fopen("dna_sequences.txt", "r");

    fscanf(input_file, "%d", string_num); //scan number of inputs
    fscanf(input_file, "%*s"); //remove unnecessary characters

    //scan the dna subsequences
    for(int i = 0; i < *string_num; i++){
        dna[i] = (DNA_SEQ*)malloc(sizeof(DNA_SEQ));
        dna[i]->sequence = malloc(sizeof(char) * MAX_LENGTH);
        fscanf(input_file, "%s", dna[i]->sequence);
        dna[i]->length = strlen(dna[i]->sequence);
        dna[i]->A = malloc(sizeof(int) * dna[i]->length);
        dna[i]->T = malloc(sizeof(int) * dna[i]->length);
        dna[i]->G = malloc(sizeof(int) * dna[i]->length);
        dna[i]->C = malloc(sizeof(int) * dna[i]->length);
        
        int a_val = -1;
        int t_val = -1;
        int g_val = -1;
        int c_val = -1;
        for(int j = dna[i]->length-1; j >= 0; j--){
            if(dna[i]->sequence[j] == 'A'){
                dna[i]->A[j] = j+1;
                dna[i]->T[j] = t_val;
                dna[i]->G[j] = g_val;
                dna[i]->C[j] = c_val;
                a_val = j+1;
            }else if(dna[i]->sequence[j] == 'T'){
                dna[i]->A[j] = a_val;
                dna[i]->T[j] = j+1;
                dna[i]->G[j] = g_val;
                dna[i]->C[j] = c_val;
                t_val = j+1;
            }else if(dna[i]->sequence[j] == 'G'){
                dna[i]->A[j] = a_val;
                dna[i]->T[j] = t_val;
                dna[i]->G[j] = j+1;
                dna[i]->C[j] = c_val;
                g_val = j+1;
            }else{
                dna[i]->A[j] = a_val;
                dna[i]->T[j] = t_val;
                dna[i]->G[j] = g_val;
                dna[i]->C[j] = j+1;
                c_val = j+1;
            }
        }
        printf("%s\n", dna[i]->sequence);
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

// write final inputs to file (aligned dna sequences and asterisks to mark lcs)
void write_final_results(char** dna, int size, char* lcs){
    int lcs_index = 0;
    char* aligned_dna[size];
    int max_length = 0;
    int indexes[5] = {0};

    // fill with dashes
    for(int i = 0; i < size; i++){
        aligned_dna[i] = malloc(sizeof(char) * 500);
        for (int j = 0; j < 500; j++) {
            aligned_dna[i][j] = '-';
        }
    }
    
    FILE *output;
    output = fopen("hw2_output.txt", "w");

    int aligned_index = 0;
    int loop = 1;
    while(loop){
        for(int i = 0; i < size; i++){
            if(dna[i][indexes[i]] != lcs[lcs_index]){
                aligned_dna[i][aligned_index] = dna[i][indexes[i]];
            }else if (lcs[lcs_index] == dna[i][indexes[i]] && is_common(dna, size, lcs, lcs_index, indexes)){ //aligned
                for(int j = 0; j < size; j++) aligned_dna[j][aligned_index] = dna[j][indexes[j]];
                lcs_index++;
                break;
            }else{
                indexes[i]--; //if not aligned, leave as dash
            } 
        }
        aligned_index++;
        for(int i = 0; i <size; i++) indexes[i]++; //increment indexes
        
        //check if indexes are within bound
        for(int i = 0; i < size; i++){
            if(indexes[i] >= strlen(dna[i])) {
                loop = 0;
                break;
            }
        }
    }

    //appending any left over characters after alignment
    int longest_aligned_length = 0;
    for(int i = 0; i < size; i++){
        if(indexes[i] < strlen(dna[i])){
            int temp_aligned_index = aligned_index;
            if((strlen(dna[i]) - indexes[i]) > longest_aligned_length) longest_aligned_length = strlen(dna[i]) - indexes[i];
            for(int j = indexes[i]; j < strlen(dna[i]); j++){
                aligned_dna[i][temp_aligned_index] = dna[i][j];
                temp_aligned_index++;
            }
        }
    }
    longest_aligned_length += aligned_index;

    // write aligned sequences to file
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < longest_aligned_length; j++) fputc(aligned_dna[i][j], output);
        fputc('\n', output);
    }

    // print asterisks 
    lcs_index = 0;
    for(int j = 0; j < aligned_index; j++){
        if(lcs_index > (strlen(lcs))) break;
        else if(lcs[lcs_index] == aligned_dna[0][j]){
            fputc('*', output);
            lcs_index++;
        }else{
            fputc(' ', output);
        }
    }
    
    for(int i = 0; i < size; i ++) free(aligned_dna[i]);
    fclose(output);
}

char* find_lcs(char** dna){
    NODE root = {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, 0};

    //initialize memoization table
    MEMO* memo_table = malloc(sizeof(MEMO) * 1000000);

}



int main(){
    //Checking run time at beginning of main
    int TIME = 0;
    clock_t start = clock();

    int string_num = 0;
    DNA_SEQ* dna[5];
    take_input(dna, &string_num); //store the input sequences
    
    /*char* lcs;

    lcs = find_lcs(dna);
    
    printf("lcs: %s\n", lcs);
    write_final_results(dna, string_num, lcs);

    //free memory
    free(lcs);*/

    for(int i = 0; i < string_num; i++) free(dna[i]->sequence);

    //at end of main
    TIME += ((int)clock() - start) / (CLOCKS_PER_SEC / 1000);
    printf("TIME : %d ms\n", TIME);
}