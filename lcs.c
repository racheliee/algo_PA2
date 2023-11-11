#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LENGTH 120
#define MAX_SEQUENCES 5

//struct definitions ===========================================================
typedef struct dna_seq{
    char* sequence;
    int length;
    int** letter; // stores the leters of each letter's indexes (0 = A, 1 = T, 2 = G, 3 = C)
} DNA_SEQ;

typedef struct node{
    int* str_indexes; //stores indexes of each letter for each sequence
    int* next_indexes; //stores the next indexes that gives the max lcs
    int lcs_length;
    int chosen; //0-A, 1-T, 2-G, 3-C
    struct node* next_node;
}NODE;

// helper functions ============================================================
// takes input
void take_input(DNA_SEQ* dna, int* string_num){
    FILE *input_file = fopen("hw2_input.txt", "r");
    //FILE *input_file = fopen("dna_sequences.txt", "r");

    fscanf(input_file, "%d", string_num); //scan number of inputs
    fscanf(input_file, "%*s"); //remove unnecessary characters

    //scan the dna subsequences
    for(int i = 0; i < *string_num; i++){
        dna[i].sequence = malloc(sizeof(char) * MAX_LENGTH);
        fscanf(input_file, "%s", dna[i].sequence);
        dna[i].length = strlen(dna[i].sequence);

        dna[i].letter = malloc(sizeof(int*) * 4); // 4 letters [A, T, G, C]
        for(int j = 0; j < 4; j++){
            dna[i].letter[j] = malloc(sizeof(int) * dna[i].length);
        }
        // fill in the letter array with indexes of each character
        int a_val = -1;
        int t_val = -1;
        int g_val = -1;
        int c_val = -1;
        for(int j = dna[i].length-1; j >= 0; j--){
            if(dna[i].sequence[j] == 'A'){
                dna[i].letter[0][j] = j+1;
                dna[i].letter[1][j] = t_val;
                dna[i].letter[2][j] = g_val;
                dna[i].letter[3][j] = c_val;
                a_val = j+1;
            }else if(dna[i].sequence[j] == 'T'){
                dna[i].letter[0][j] = a_val;
                dna[i].letter[1][j] = j+1;
                dna[i].letter[2][j] = g_val;
                dna[i].letter[3][j] = c_val;
                t_val = j+1;
            }else if(dna[i].sequence[j] == 'G'){
                dna[i].letter[0][j] = a_val;
                dna[i].letter[1][j] = t_val;
                dna[i].letter[2][j] = j+1;
                dna[i].letter[3][j] = c_val;
                g_val = j+1;
            }else{
                dna[i].letter[0][j] = a_val;
                dna[i].letter[1][j] = t_val;
                dna[i].letter[2][j] = g_val;
                dna[i].letter[3][j] = j+1;
                c_val = j+1;
            }
        }
    }

    fclose(input_file);
}


// returns 1 if the values are the same at the given index
int is_common(DNA_SEQ* dna, int size, char* lcs, int lcs_index, int indexes[5]){
    for(int i = 0; i < size; i++){
        if(dna[i].sequence[indexes[i]] != lcs[lcs_index]){
            return 0;
        }
    }
    return 1;
}

// write final inputs to file (aligned dna sequences and asterisks to mark lcs)
void write_final_results(DNA_SEQ* dna, int size, char* lcs){
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
            if(dna[i].sequence[indexes[i]] != lcs[lcs_index]){
                aligned_dna[i][aligned_index] = dna[i].sequence[indexes[i]];
            }else if (lcs[lcs_index] == dna[i].sequence[indexes[i]] && is_common(dna, size, lcs, lcs_index, indexes)){ //aligned
                for(int j = 0; j < size; j++) aligned_dna[j][aligned_index] = dna[j].sequence[indexes[j]];
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
            if(indexes[i] >= dna[i].length) {
                loop = 0;
                break;
            }
        }
    }

    //appending any left over characters after alignment
    int longest_aligned_length = 0;
    for(int i = 0; i < size; i++){
        if(indexes[i] < dna[i].length){
            int temp_aligned_index = aligned_index;
            if((dna[i].length - indexes[i]) > longest_aligned_length) longest_aligned_length = dna[i].length - indexes[i];
            for(int j = indexes[i]; j < dna[i].length; j++){
                aligned_dna[i][temp_aligned_index] = dna[i].sequence[j];
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


// lcs functions ================================================================
int calc_lcs_length4(DNA_SEQ* dna, int string_num, NODE***** table, int* indexes){
    // if the indexes equal to -1, it indicated the end of string and hence, the length is 0
    for(int i = 0; i < string_num; i++){
        if(indexes[i] == -1) return 0;
    }
    
    //if the indexes are longer than the string, the string has ended. return 0
    for(int i = 0; i < string_num; i++){
        if(indexes[i] >= dna[i].length) return 0;
    }

    //check
    printf("indexes: ");
    for(int i= 0; i < string_num; i++) printf("%d, ", indexes[i]);
    printf("\n");

    // if indexes are valid, check if the value is already in the table
    NODE* current_node = table[indexes[0]][indexes[1]][indexes[2]][indexes[3]];

    // if the value is already in the table, then return its length
    if(current_node != NULL){
        return current_node->lcs_length;
    }
    //otherwise append that node to the memoization table
    else{
        NODE *new_node = (NODE*)malloc(sizeof(NODE));
        new_node->str_indexes = malloc(sizeof(int) * string_num);

        //save the indexes as the new node's str_indexes
        for(int i = 0; i < string_num; i++){
            new_node->str_indexes[i] = indexes[i];
        }

        //calculate the lcs length by looking at the next four nodes
        int next_four_indexes[4][string_num];
        for(int i = 0; i < string_num; i++){
            for(int j = 0; j < 4; j++){
                next_four_indexes[j][i] = dna[i].letter[j][indexes[i]];
            }
        }

        //check
        for(int i = 0; i < 4; i++){
            printf("next four indexes %d: ", i);
            for(int j = 0; j < string_num; j++){
                printf("%d ", next_four_indexes[i][j]);
            }
            printf("\n");
        }

        int next_four_node_lengths[4];
        for(int i = 0; i < 4; i++){
            next_four_node_lengths[i] = calc_lcs_length4(dna, string_num, table, next_four_indexes[i]);
        }

        //find the max length
        int max_length = next_four_node_lengths[0];
        for(int i = 1; i < 4; i++){
            if(next_four_node_lengths[i] > max_length) {
                max_length = next_four_node_lengths[i];
            }
        }

        table[indexes[0]][indexes[1]][indexes[2]][indexes[3]] = new_node;
        new_node->lcs_length = max_length+1; 

        return max_length+1; //+1 becaue the current node is also included
    }
}

char* traceback4(NODE root, NODE***** table){

}

char* find_lcs4(DNA_SEQ* dna, int string_num){
    // creating memoization table (4D array)
    NODE***** table = (NODE*****)malloc(sizeof(NODE****) * MAX_LENGTH);

    for(int i = 0; i < MAX_LENGTH; i++){
        table[i] = (NODE****)malloc(sizeof(NODE***) * MAX_LENGTH);
        for(int j = 0; j < MAX_LENGTH; j++){
            table[i][j] = (NODE***)malloc(sizeof(NODE**) * MAX_LENGTH);
            for(int k = 0; k < MAX_LENGTH; k++){
                table[i][j][k] = (NODE**)malloc(sizeof(NODE*) * MAX_LENGTH);
                for(int l = 0; l < MAX_LENGTH; l++){
                    table[i][j][k][l] = NULL;
                }
            }
        }
    }
    //starting index is all 0's
    int* starting_indexes = malloc(sizeof(int) * string_num);
    for(int i = 0; i < string_num; i++) starting_indexes[i] = 0;

    int lcs_length = calc_lcs_length4(dna, string_num, table, starting_indexes);
    printf("lcs len: %d\n", lcs_length);


    char* lcs = "ATTGCCAT";

    return lcs;
}

// main function ================================================================
int main(){
    //Checking run time at beginning of main
    int TIME = 0;
    clock_t start = clock();

    int string_num = 0;
    DNA_SEQ dna[MAX_SEQUENCES];
    take_input(dna, &string_num); //store the input sequences

    for(int i = 0; i < string_num; i++){
        for(int j = 0; j < 4; j++){
            for(int k = 0; k < dna[i].length; k++) printf("%d", dna[i].letter[j][k]);
            printf("\n");
        }
        printf("%s\n\n", dna[i].sequence);
    }

    char* lcs;

    if(string_num == 2){
        
    }else if(string_num == 3){
        
    }else if(string_num == 4){
        lcs = find_lcs4(dna, string_num);
    }else{
        
    }

    write_final_results(dna, string_num, lcs);

    for(int i = 0; i < string_num; i++) free(dna[i].sequence);

    //at end of main
    TIME += ((int)clock() - start) / (CLOCKS_PER_SEC / 1000);
    printf("TIME : %d ms\n", TIME);
    return 0;
}
