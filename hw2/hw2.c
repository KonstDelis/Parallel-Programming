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

int rows, columns;
int YEARS=0;

int getNeigborNumber(char** table, int row, int column){
    int neighbors=0;
    assert(row>=0 && row<rows);
    assert(table);
    assert(column>=0 && column<columns);

    if(row-1>=0){
        if(column-1>=0){
            neighbors += table[row-1][column-1]=='*'? 1:0;
            neighbors += table[row][column-1]=='*'? 1:0;
        }
        if(column+1<columns){
            neighbors += table[row-1][column+1]=='*'? 1:0;
            neighbors += table[row][column+1]=='*'? 1:0;
        }
        neighbors += table[row-1][column]=='*'? 1:0;
    }
    if(row+1<rows){
        if(column-1>=0)
            neighbors += table[row+1][column-1]=='*'? 1:0;
        if(column+1<columns)
            neighbors += table[row+1][column+1]=='*'? 1:0;
        neighbors += table[row+1][column]=='*'? 1:0;
    }
    return neighbors;
}

void change_state(char** current_year, char** next_year, int row, int column){
    assert(row>=0 && row<rows);
    assert(column>=0 && column<columns);

    int neighbors = getNeigborNumber(current_year, row,column);
    if(current_year[row][column]==ALIVE)
        next_year[row][column] = (neighbors<=SOLITUDE || neighbors>=OVERPOPULATION)? DEAD:ALIVE;
    else
        next_year[row][column] = (neighbors==CAN_SPAWN)? ALIVE:DEAD;
}

char** run_years(char** tableA, char** tableB){
    int i=0, j=0;
    char** current_year, **next_year, **tmp;
    current_year = tableA;
    next_year = tableB;
    #pragma omp parallel
    {
        for(int y=1; y<=YEARS; y++){

            #pragma omp for private(i,j)
            for(i=0; i<rows; i++){
                for(j=0; j<columns; j++){
                    change_state(current_year, next_year, i,j);
                }
            }

            #pragma omp barrier

            #pragma omp single
            {
                tmp = next_year;
                next_year = current_year;
                current_year = tmp;
            }
            #pragma omp barrier
        }
    }
    return next_year;
}

void read_input(FILE* fin, char** mainTable){
    assert(fin);
    assert(mainTable);
    for(int i=0; i<rows; i++){
        fscanf(fin,"|");
        for(int j=0; j<columns; j++){
            fscanf(fin,"%c|",&mainTable[i][j]);
        }
        fscanf(fin,"\n");
    }
}

void print_table(FILE* fout, char** result){
    assert(fout);
    for(int i=0; i<rows; i++){
        fprintf(fout,"|");
        for(int j=0; j<columns; j++){
            fprintf(fout,"%c|",result[i][j]);
        }
        fprintf(fout,"\n");
    }
}

char** initialize_table(char** table){
    table = malloc(sizeof(char*)*rows);
    for(int i=0; i<rows; i++){
        table[i] = malloc(sizeof(char)*columns);
    }

    return table;
}

void delete_tables(char** tableA, char** tableB){
    for(int i=0; i<rows; i++){
        free(tableA[i]);
        free(tableB[i]);
    }
    free(tableA);
    free(tableB);
}

int main(int argc, char* argv[]){
    char **tableA, **tableB, **result;
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
        fout = fopen(argv[3],"w+");
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
        exit(1);
    }

    fscanf(fin, "%d %d\n",&rows, &columns);

    tableA = initialize_table(tableA);
    tableB = initialize_table(tableB);

    read_input(fin, tableA);

    result = run_years(tableA, tableB);

    print_table(fout, result);

    delete_tables(tableA, tableB);

    fclose(fin);
    fclose(fout);
}