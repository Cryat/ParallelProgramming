#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <signal.h>
#include <pthread.h>
#include "ESCRITOR.h"
#define BUF_SIZE 1000 
#define STDFD 0

// to be created: MONITOR.h

int main(){
    int writerpid, readerpid;
    int pipeline[2];
    char buffer[14];
    int exitFlag = 1;
    
    
    
        if ((writerpid=fork())==-1) 
            printf("Failed to create Writer process\n");
        
        else if (writerpid == 0) {
                if(execlp("./ESCRITOR","./ESCRITOR", NULL )==-1){
                    printf("Failed to launch Writer process\n");
                    return -1;
                }
                return 0;
        }
        
        if (pipe(pipeline) == -1) 
            printf("Could not create pipe\n");
        
        
        if ((readerpid=fork())==-1) 
            printf("Failed to create Reader process\n");
    
        else if (readerpid == 0) {
                if ((close(STDFD) == -1) || (dup(pipeline[0]) == -1))
                    printf("Reader could not redirect input to pipe\n");
                if (close(pipeline[1]) == -1)
                    printf("Reader: Could not close output pipe\n");
                if(execlp("./LEITOR", "./LEITOR", NULL)==-1) {
                    printf("Failed to launch Reader process\n");
                    return -1;
                }
                return 0;
        }
         

    if (close(pipeline[0])==-1)
        printf("Monitor: could not close input pipe\n");
    
    while(exitFlag!=0) {
        int size = read(STDFD, buffer, sizeof(char)*13);
        
        if (size == -1)
            printf("Monitor input read did not succeed\n");
        else if ((size > 0) && (buffer[size-1] == '\n' || buffer[size-1] == ' '))
            buffer[size-1] = '\0';
        else
            buffer[size] = '\0';
            
        if (strcmp(buffer, "il") == 0)
            kill(writerpid, SIGUSR1);
        else if (strcmp(buffer, "ie") == 0)
            kill(writerpid, SIGUSR2);
        else if (strcmp(buffer, "sair") == 0)
            exitFlag = 0;
        else if(write(pipeline[1], buffer, size) == -1){
            printf("Could not write into the pipe output \n");
        }
            
    }
    
    if (close(pipeline[1]) == -1)
        printf("Monitor: could not close output pipe\n");
    
    kill(writerpid, SIGTSTP);

    while(wait(NULL) != -1);
        
    return 0;
}