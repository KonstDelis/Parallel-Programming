#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <assert.h>
#include <unistd.h>

#define OVERPOPULATION 4
#define SOLITUDE 1
#define CAN_SPAWN 3
#define YEARS 100
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

void change_state(int row, int column){
    table[row][column].current = table[row][column].next;
}

void run_years(){
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
        printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
        print_table_colored();
        sleep(1);
    }
}

void green(){
    printf(GREEN);
}

void red(){
    printf(RED);
}

void reset(){
    printf(RESET);
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

int main(int argc, char* argv[]){
    assert(argv);
    /*Check arguments*/
    if((argc!=5 && argc!=3) || strcmp(argv[1],"-in")!=0 ){
        fprintf(stderr, "Error: invalid parameters. Run program as: ./executable -in input_file.txt -out output_file.txt. If you skip output file parameters stdout will be used\n");
        exit(1);
    }
    
    /*open files*/
    FILE *fin = fopen(argv[2], "r");
    FILE* fout = stdout;
    if((argc==5)){
        if(strcmp(argv[3],"-out")==0){
            fprintf(stderr, "Error: invalid parameters. Run program as: ./executable -in input_file.txt -out output_file.txt. If you skip output file parameters stdout will be used\n");
            exit(1);
        }
        fopen(argv[4],"w");
    }
    if (fin == NULL){
        fprintf(stderr,"Error: Cannot open input file\n");
        return -1;
    }
    if(fout==NULL){
        fprintf(stderr, "Error: Cannot open output file\n");
        return -1;
    }
    read_input(fin);
    for(int i=0; i<rows; i++){
        for(int j=0; j<columns; j++){
            if(table[i][j].current=='*')
                printf("[%d][%d]: has %d neighbors\n",i,j,getNeigborNumber(i,j));
        }
    }
    run_years();
    
    for(int i=0; i<rows; i++){
        free(table[i]);
    }
    free(table);
}