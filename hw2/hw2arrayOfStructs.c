#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <assert.h>
#include <unistd.h>

#define OVERPOPULATION 4
#define SOLITUDE 1
#define CAN_SPAWN 3
#define ALIVE '*'
#define DEAD ' '

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"

typedef struct cell{
    char current;
    char next;
} cell_t;

int rows, columns;
cell_t** table;
int YEARS=0;

void green(){
    printf(GREEN);
}

void red(){
    printf(RED);
}

void reset(){
    printf(RESET);
}

int getNeigborNumber(int row, int column){
    int neighbors=0;
    assert(row>=0 && row<rows);
    assert(column>=0 && column<columns);

    if(row-1>=0){
        if(column-1>=0){
            neighbors += table[row-1][column-1].current=='*'? 1:0;
            neighbors += table[row][column-1].current=='*'? 1:0;
        }
        if(column+1<columns){
            neighbors += table[row-1][column+1].current=='*'? 1:0;
            neighbors += table[row][column+1].current=='*'? 1:0;
        }
        neighbors += table[row-1][column].current=='*'? 1:0;
    }
    if(row+1<rows){
        if(column-1>=0)
            neighbors += table[row+1][column-1].current=='*'? 1:0;
        if(column+1<columns)
            neighbors += table[row+1][column+1].current=='*'? 1:0;
        neighbors += table[row+1][column].current=='*'? 1:0;
    }
    return neighbors;
}

void calculate_next_state(int row, int column){
    assert(row>=0 && row<rows);
    assert(column>=0 && column<columns);

    int neighbors = getNeigborNumber(row,column);
    if(table[row][column].current==ALIVE)
        table[row][column].next = (neighbors<=SOLITUDE || neighbors>=OVERPOPULATION)? DEAD:ALIVE;
    else
        table[row][column].next = (neighbors==CAN_SPAWN)? ALIVE:DEAD;
}

void print_table_colored(){
    for(int i=0; i<rows; i++){
        printf("|");
        for(int j=0; j<columns; j++){
            green();
            printf("%c",table[i][j].current);
            reset();
            printf("|");
        }
        printf("\n");
    }
}

void change_state(int row, int column){
    table[row][column].current = table[row][column].next;
}

void run_years_animated(){
    for(int y=1; y<=YEARS; y++){
        for(int i=0; i<rows; i++){
            for(int j=0; j<columns; j++){
                calculate_next_state(i,j);
            }
        }
        for(int i=0; i<rows; i++){
            for(int j=0; j<columns; j++){
                change_state(i,j);
            }
        }
        
        print_table_colored();
        usleep(10000);
    }
}

void run_years(){
    int i=0, j=0;
    #pragma omp parallel 
    {
        for(int y=1; y<=YEARS; y++){

            #pragma omp for private(i,j)
            for(i=0; i<rows; i++){
                for(j=0; j<columns; j++){
                    calculate_next_state(i,j);
                }
            }
            #pragma omp barrier
            #pragma omp for private(i,j)
            for(i=0; i<rows; i++){
                for(j=0; j<columns; j++){
                    change_state(i,j);
                }
            }
            #pragma omp barrier
        }
    }
}

void read_input(FILE* fin){
    assert(fin);
    fscanf(fin, "%d %d\n",&rows, &columns);
    table = malloc(sizeof(cell_t*)*rows);
    if(table==NULL){
        fprintf(stderr, "Error: malloc failed line:%d\n",__LINE__);
    }
    for(int i=0; i<rows; i++){
        table[i] = malloc(sizeof(cell_t)*columns);
        if(table[i]==NULL){
            fprintf(stderr, "Error: malloc failed line:%d\n",__LINE__);
        }
    }

    for(int i=0; i<rows; i++){
        fscanf(fin,"|");
        for(int j=0; j<columns; j++){
            fscanf(fin,"%c|",&(table[i][j].current));
        }
        fscanf(fin,"\n");
    }
}

void print_table(FILE* fout){
    assert(fout);
    for(int i=0; i<rows; i++){
        fprintf(fout,"|");
        for(int j=0; j<columns; j++){
            fprintf(fout,"%c|",table[i][j].current);
        }
        fprintf(fout,"\n");
    }
}

int main(int argc, char* argv[]){
    assert(argv);
    /*Check arguments*/
    if(argc!=4 && argc!=3){
        fprintf(stderr, "Error: invalid parameters. Run program as: ./executable -in input_file.txt -out output_file.txt. If you skip output file parameters stdout will be used\n");
        exit(1);
    }
    
    /*open files*/
    FILE *fin = fopen(argv[1], "r");
    FILE* fout = stdout;
    if((argc==4)){
        fopen(argv[3],"w");
    }
    if (fin == NULL){
        fprintf(stderr,"Error: Cannot open input file\n");
        return -1;
    }
    if(fout==NULL){
        fprintf(stderr, "Error: Cannot open output file\n");
        return -1;
    }
    YEARS = atoi(argv[2]);
    if (YEARS<=0) {
        fprintf(stderr, "Error: years cannot be smaller than ot equal to 0\n");
    }

    read_input(fin);

    run_years();

    print_table(fout);
    
    for(int i=0; i<rows; i++){
        free(table[i]);
    }
    free(table);
}