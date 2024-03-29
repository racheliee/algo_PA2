#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LENGTH 120

typedef struct dna_seq{
    char* sequence;
    int length;
} DNA_SEQ;

// helper functions ============================================================
// takes input
void take_input(DNA_SEQ** dna, int* string_num){
    //FILE *input_file = fopen("hw2_input.txt", "r");
    FILE *input_file = fopen("dna_sequences.txt", "r");

    //scan number of inputs
    fscanf(input_file, "%d", string_num); 

    //remove unnecessary characters
    fscanf(input_file, "%*s"); 

    //scan the dna subsequences
    for(int i = 0; i < *string_num; i++){
        dna[i] = (DNA_SEQ*)malloc(sizeof(DNA_SEQ));
        dna[i]->sequence = malloc(sizeof(char) * MAX_LENGTH);
        fscanf(input_file, "%s", dna[i]->sequence);
        dna[i]->length = strlen(dna[i]->sequence);
    }

    fclose(input_file);
}

// returns 1 if the values are the same at the given index
int is_common(DNA_SEQ** dna, int size, char* lcs, int lcs_index, int indexes[5]){
    for(int i = 0; i < size; i++){
        if(dna[i]->sequence[indexes[i]] != lcs[lcs_index]){
            return 0;
        }
    }
    return 1;
}

// write final inputs to file (aligned dna sequences and asterisks to mark lcs)
void write_final_results(DNA_SEQ** dna, int size, char* lcs){
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
            if(dna[i]->sequence[indexes[i]] != lcs[lcs_index]){
                aligned_dna[i][aligned_index] = dna[i]->sequence[indexes[i]];
            }else if (lcs[lcs_index] == dna[i]->sequence[indexes[i]] && is_common(dna, size, lcs, lcs_index, indexes)){ //aligned
                for(int j = 0; j < size; j++) aligned_dna[j][aligned_index] = dna[j]->sequence[indexes[j]];
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
            if(indexes[i] >= dna[i]->length) {
                loop = 0;
                break;
            }
        }
    }

    //appending any left over characters after alignment
    int longest_aligned_length = 0;
    for(int i = 0; i < size; i++){
        if(indexes[i] < dna[i]->length){
            int temp_aligned_index = aligned_index;
            if((dna[i]->length - indexes[i]) > longest_aligned_length) longest_aligned_length = dna[i]->length - indexes[i];
            for(int j = indexes[i]; j < dna[i]->length; j++){
                aligned_dna[i][temp_aligned_index] = dna[i]->sequence[j];
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

//returns max value of 5 integers
int get_max(int a, int b, int c, int d, int e){
    int max = a;
    int arr[5] = {a, b, c, d, e};

    for(int i = 0; i < 5; i++){
        if(arr[i] > max){
            max = arr[i];
        }
    }
    return max;
}

// lcs functions ================================================================
char* find_lcs2(DNA_SEQ** dna){
    int len1 = dna[0]->length;
    int len2 = dna[1]->length;

    // create table
    int** table = (int**)malloc(sizeof(int*) * (len1+1));
    for(int i = 0; i < len1+1; i++){
        table[i] = (int*)malloc(sizeof(int) * (len2+1));
    }

    // fill in table
    for(int i = 0; i < len1+1; i++){
        for(int j = 0; j < len2+1; j++){
            if(i == 0 || j == 0){
                table[i][j] = 0;
            }else if(dna[0]->sequence[i-1] == dna[1]->sequence[j-1]){
                table[i][j] = table[i-1][j-1] + 1;
            }else{
                table[i][j] = get_max(table[i-1][j], table[i][j-1], 0, 0, 0);
            }
        }
    }

    // create lcs
    int lcs_length = table[len1][len2];
    char* lcs = malloc(sizeof(char) * lcs_length);
    lcs[lcs_length] = '\0';

    // backtracking to find lcs
    int lcs_index = lcs_length-1;
    int i = len1; int j = len2;
    while(i > 0 && j > 0){
        if(table[i-1][j] == table[i][j]){
            i--;
        }else if(table[i][j-1] == table[i][j]){
            j--;
        }else{
            lcs[lcs_index] = dna[0]->sequence[i-1];
            i--; j--;
            lcs_index--;
        }
    }

    free(table);

    return lcs;
}

char* find_lcs3(DNA_SEQ** dna){
    int len1 = dna[0]->length; int len2 = dna[1]->length; int len3 = dna[2]->length;

    // create table
    int*** table = (int ***)malloc(sizeof(int**) * (len1+1));
    
    for(int i = 0; i < len1+1; i++){
        table[i] = (int**)malloc(sizeof(int*) * (len2+1));
        for(int j = 0; j < len2+1; j++){
            table[i][j] = (int*)malloc(sizeof(int) * (len3+1));
        }
    }

    // fill in table
    for(int i = 0; i < len1+1; i ++){
        for(int j = 0; j < len2+1; j++){
            for(int k = 0; k < len3+1; k++){   
                if(i == 0 || j == 0 || k == 0){
                    table[i][j][k] = 0;
                }else if (dna[0]->sequence[i-1] == dna[1]->sequence[j-1] && dna[1]->sequence[j-1] == dna[2]->sequence[k-1]){
                    table[i][j][k] = table[i-1][j-1][k-1] + 1;
                }else{
                    table[i][j][k] = get_max(table[i-1][j][k], table[i][j-1][k], table[i][j][k-1], 0, 0);
                }
            }
        }
    }

    // create lcs
    int lcs_length = table[len1][len2][len3];
    char* lcs = malloc(sizeof(char) * lcs_length);
    lcs[lcs_length] = '\0';

    // backtracking to find lcs
    int i = len1; int j = len2; int k = len3;
    int lcs_index = lcs_length-1;
    while(i > 0 && j > 0 && k > 0){
        if(table[i-1][j][k] == table[i][j][k]){
            i--;
        }else if(table[i][j-1][k] == table[i][j][k]){
            j--;
        }else if(table[i][j][k-1] == table[i][j][k]){
            k--;
        }else{
            lcs[lcs_index] = dna[0]->sequence[i-1];
            i--; j--; k--;
            lcs_index--;
        }
    }

    free(table);

    return lcs;
}

char* find_lcs4(DNA_SEQ** dna){
    int len1 = dna[0]->length; int len2 = dna[1]->length; int len3 = dna[2]->length; int len4 = dna[3]->length; 
    
    // create table
    int**** table = (int****)malloc(sizeof(int***) * (len1+1));

    for(int i = 0; i < len1+1; i++){
        table[i] = (int***)malloc(sizeof(int**) * (len2+1));
        for(int j = 0; j < len2+1; j++){
            table[i][j] = (int**)malloc(sizeof(int*) * (len3+1));
            for(int k = 0; k < len3+1; k++){
                table[i][j][k] = (int*)malloc(sizeof(int) * (len4+1));
            }
        }
    }
    
    // fill in table
    for(int i = 0; i < len1+1; i++){
        for(int j = 0; j < len2+1; j++){
            for(int k = 0; k < len3+1; k++){
                for(int l= 0; l < len4+1; l++){
                    if(i == 0 || j == 0 || k == 0|| l == 0){
                        table[i][j][k][l] = 0;
                    }else if(dna[0]->sequence[i-1] == dna[1]->sequence[j-1] && dna[1]->sequence[j-1] == dna[2]->sequence[k-1] && dna[2]->sequence[k-1] == dna[3]->sequence[l-1]){
                        table[i][j][k][l] = table[i-1][j-1][k-1][l-1] + 1;
                    }else{
                        table[i][j][k][l] = get_max(table[i-1][j][k][l], table[i][j-1][k][l], table[i][j][k-1][l], table[i][j][k][l-1], 0);
                    }
                }
            }
        }
    }

    // create lcs
    int lcs_length = table[len1][len2][len3][len4];
    char* lcs = malloc(sizeof(char) * lcs_length);
    lcs[lcs_length] = '\0';

    // backtracking to find lcs
    int lcs_index = lcs_length-1;
    int i = len1; int j = len2; int k = len3; int l = len4;
    while(i > 0 && j > 0 && k > 0 && l > 0){
        int temp = table[i][j][k][l];
        if(table[i-1][j][k][l] == temp){
            i--;
        }else if(table[i][j-1][k][l] == temp){
            j--;
        }else if(table[i][j][k-1][l] == temp){
            k--;
        }else if(table[i][j][k][l-1] == temp){
            l--;
        }else{
            lcs[lcs_index] = dna[0]->sequence[i-1];
            lcs_index--;
            i--; j--; k--; l--;
        }
    }
    
    free(table);

    return lcs;
}

char* find_lcs5(DNA_SEQ** dna){
    int len1 = dna[0]->length; int len2 = dna[1]->length; int len3 = dna[2]->length; int len4 = dna[3]->length; int len5 = dna[4]->length;

    // create table
    int***** table = (int*****)malloc(sizeof(int****) * (len1+1));

    for(int i = 0; i < len1+1; i++){
        table[i] = (int****)malloc(sizeof(int***) * (len2+1));
        for(int j = 0; j < len2+1; j++){
            table[i][j] = (int***)malloc(sizeof(int**) * (len3+1));
            for(int k = 0; k < len3+1; k++){
                table[i][j][k] = (int**)malloc(sizeof(int*) * (len4+1));
                for(int l = 0; l < len4+1; l++){
                    table[i][j][k][l] = (int*)malloc(sizeof(int) * (len5+1));
                }
            }
        }
    }

    // fill in table
    for(int i = 0; i < len1+1; i++){
        for(int j = 0; j < len2+1; j++){
            for(int k = 0; k < len3+1; k++){
                for(int l = 0; l < len4+1; l++){
                    for(int m = 0; m < len5+1; m++){
                        if(i == 0 || j == 0 || k == 0 || l == 0 || m == 0){
                            table[i][j][k][l][m] = 0;
                        }else if(dna[0]->sequence[i-1] == dna[1]->sequence[j-1] && dna[1]->sequence[j-1] == dna[2]->sequence[k-1] 
                                && dna[2]->sequence[k-1] == dna[3]->sequence[l-1] && dna[3]->sequence[l-1] == dna[4]->sequence[m-1]){
                            table[i][j][k][l][m] = table[i-1][j-1][k-1][l-1][m-1] + 1;
                        }else{
                            table[i][j][k][l][m] = get_max(table[i-1][j][k][l][m], table[i][j-1][k][l][m], table[i][j][k-1][l][m], table[i][j][k][l-1][m], table[i][j][k][l][m-1]);
                        }
                    }
                }
            }
        }
    }

    // create lcs
    int lcs_length = table[len1][len2][len3][len4][len5];
    char* lcs = malloc(sizeof(char) * lcs_length);
    lcs[lcs_length] = '\0';

    // backtracking to find lcs
    int lcs_index = lcs_length-1;
    int i = len1; int j = len2; int k = len3; int l = len4; int m = len5;
    while(i > 0 && j > 0 && k > 0 && l > 0 && m > 0){
        int temp = table[i][j][k][l][m];
        if(table[i-1][j][k][l][m] == temp){
            i--;
        }else if(table[i][j-1][k][l][m] == temp){
            j--;
        }else if(table[i][j][k-1][l][m] == temp){
            k--;
        }else if(table[i][j][k][l-1][m] == temp){
            l--;
        }else if(table[i][j][k][l-1][m-1] == temp){
            m--;
        }else{
            lcs[lcs_index] = dna[0]->sequence[i-1];
            lcs_index--;
            i--; j--; k--; l--; m--;
        }
    }

    free(table);

    return lcs;
}

char* find_lcs5_alternative(DNA_SEQ** dna){
    return NULL;
}

int main(){
    //Checking run time
    //at beginning of main
    int TIME = 0;
    clock_t start = clock();

    int string_num = 0;
    DNA_SEQ* dna[5];
    take_input(dna, &string_num); //store the input sequences
    char* lcs;

    if(string_num == 2){
        lcs = find_lcs2(dna);
    }else if(string_num == 3){
        lcs = find_lcs3(dna);
    }else if(string_num == 4){
        lcs = find_lcs4(dna);
    }else{
        if(get_max(dna[0]->length, dna[1]->length, dna[2]->length, dna[3]->length, dna[4]->length) < 80) lcs = find_lcs5(dna);
        else lcs = find_lcs5_alternative(dna);
    }
    
    printf("lcs: %s\n", lcs);
    write_final_results(dna, string_num, lcs);

    //free memory
    free(lcs);
    for(int i = 0; i < string_num; i++) free(dna[i]);

    //at end of main
    TIME += ((int)clock() - start) / (CLOCKS_PER_SEC / 1000);
    printf("TIME : %d ms\n", TIME);
}