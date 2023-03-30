#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){
    FILE* fout = freopen("mesurements.txt", "w+", stdout);
    dup2(1,2);
    pid_t pid;

    // printf("\n\n############################################# Serial 1000 glid1.txt #############################################\n\n");
    // fflush(stdout);
    // for(int i=0; i<=5; i++){
    //     printf("\nMesurement %d:\n", i);
    //     fflush(stdout);
    //     pid_t pid = fork();
    //     if(pid==0)
    //         execlp("time", "time", "./hw2serial.out", "tests/glid1.txt", "1000", "out.txt", NULL);
    //     else if(pid>0){
    //         waitpid(-1, NULL, WUNTRACED);
    //     }   
    //     else{
    //         fprintf(stderr, "Error: fork failed\n");
    //     }
    //     fflush(stderr);
    // }  

    pid = fork();
    if(pid==0)
        execlp("export", "export", "OMP_NUM_THREADS=4", NULL);
    else if(pid>0){
        waitpid(-1, NULL, WUNTRACED);
    }   
    else{
        fprintf(stderr, "Error: fork failed\n");
    }

    printf("\n\n############################################# 4 Threads 1000 glid1.txt #############################################\n\n");
    fflush(stdout);
    for(int i=0; i<=5; i++){
        printf("\nMesurement %d:\n", i);
        fflush(stdout);
        pid = fork();
        if(pid==0)
            execlp("time", "time", "./hw2parallel.out", "tests/glid1.txt", "1000", "out.txt", NULL);
        else if(pid>0){
            waitpid(-1, NULL, 0);
        }   
        else{
            fprintf(stderr, "Error: fork failed\n");
        }
        fflush(stderr);
    }  

     
}