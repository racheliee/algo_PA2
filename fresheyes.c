#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 120

// helper functions ============================================================
// takes input
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

    // append elements to aligned dna one by one according to the lcs alignment
    int aligned_index = 0;
    int count = 0; //count the number of strings that are longer than the lcs
    while(count < size){    
        for(int i = 0; i < size; i++){
            if(indexes[i] > strlen(dna[i])){ //if longer than the string, continue
                count++;
                aligned_index--;
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
        for(int i = 0; i< size; i++) indexes[i]++;
    }

    //write aligned sequences to file
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < strlen(aligned_dna[0]); j++){
            fputc(aligned_dna[i][j], output);
        }
        fputc('\n', output);
        printf("%s\n", aligned_dna[i]);
    }

    //print asterisks
    int i = 0;
    for(int j = 0; j < strlen(aligned_dna[0]); j++){
        if(i < strlen(lcs) && lcs[i] == aligned_dna[0][j]){
            fputc('*', output);
            i++;
        }else{
            fputc(' ', output);
        }
    }

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
char* find_lcs2(char** dna){
    int len1 = strlen(dna[0]);
    int len2 = strlen(dna[1]);

    int table[len1+1][len2+1];

    for(int i = 0; i < len1+1; i++){
        for(int j = 0; j < len2+1; j++){
            if(i == 0 || j == 0){
                table[i][j] = 0;
            }else if(dna[0][i-1] == dna[1][j-1]){
                table[i][j] = table[i-1][j-1] + 1;
            }else{
                table[i][j] = get_max(table[i-1][j], table[i][j-1], 0, 0, 0);
            }
        }
    }

    int lcs_length = table[len1][len2];
    char* lcs = malloc(sizeof(char) * lcs_length);
    lcs[lcs_length] = '\0';

    int i = len1; int j = len2;
    int lcs_index = lcs_length -1;
    while(i > 0 && j > 0){
        if(table[i-1][j] == table[i][j]) {
            i--;
        }
        else if (table[j-1][i] == table[i][j]) {
            j--;
        }
        else{
            lcs[lcs_index] = dna[0][i-1];
            i--; j--;
            lcs_index--;
        }
    }

    return lcs;
}

char* find_lcs3(char** dna){
    int len1 = strlen(dna[0]);
    int len2 = strlen(dna[1]);
    int len3 = strlen(dna[2]);

    int table[len1+1][len2+1][len3+1];

    for(int i = 0; i < len1+1; i ++){
        for(int j = 0; j < len2+1; j++){
            for(int k = 0; k < len3+1; k++){   
                if(i == 0 || j == 0 || k == 0){
                    table[i][j][k] = 0;
                }else if (dna[0][i-1] == dna[1][j-1] && dna[1][j-1] == dna[2][k-1]){
                    table[i][j][k] = table[i-1][j-1][k-1] + 1;
                }else{
                    table[i][j][k] = get_max(table[i-1][j][k], table[i][j-1][k], table[i][j][k-1], 0, 0);
                }
            }
        }
    }

    int lcs_length = table[len1][len2][len3];
    char* lcs = malloc(sizeof(char) * lcs_length);
    lcs[lcs_length] = '\0';

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
            lcs[lcs_index] = dna[0][i-1];
            i--; j--; k--;
            lcs_index--;
        }
    }

    return lcs;
}

char* find_lcs4(char** dna){
    int len1 = strlen(dna[0]); int len2 = strlen(dna[1]); int len3 = strlen(dna[2]); int len4 = strlen(dna[3]);

    int table[len1+1][len2+1][len3+1][len4+1];

    for(int i = 0; i < len1+1; i++){
        for(int j = 0; j < len2+1; j++){
            for(int k = 0; k < len3+1; k++){
                for(int l= 0; l < len4+1; l++){
                    if(i == 0 || j == 0 || k == 0|| l == 0){
                        table[i][j][k][l] = 0;
                    }else if(dna[0][i-1] == dna[1][j-1] && dna[1][j-1] == dna[2][k-1] && dna[2][k-1] == dna[3][l-1]){
                        table[i][j][k][l] = table[i-1][j-1][k-1][l-1] + 1;
                    }else{
                        table[i][j][k][l] = get_max(table[i-1][j][k][l], table[i][j-1][k][l], table[i][j][k-1][l], table[i][j][k][l-1], 0);
                    }
                }
            }
        }
    }

    int lcs_length = table[len1][len2][len3][len4];
    char* lcs = malloc(sizeof(char) * lcs_length);
    lcs[lcs_length] = '\0';

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
            lcs[lcs_index] = dna[0][i-1];
            lcs_index--;
            i--; j--; k--; l--;
        }
    }

    return lcs;
}

int main(){
    int string_num = 0;
    char* dna[5];
    take_input(dna, &string_num); //store the input sequences
    char* lcs;

    if(string_num == 2){
        lcs = find_lcs2(dna);
    }else if(string_num == 3){
        lcs = find_lcs3(dna);
    }else if(string_num == 4){
        lcs = find_lcs4(dna);
    }
    
    printf("lcs: %s\n", lcs);
    write_final_results(dna, string_num, lcs);
}