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

void usr1_handler(int signo) {

	if(signo == SIGUSR1){
		lockFlag = !lockFlag;
	}
}

void usr2_handler(int signo) {

	if (signo == SIGUSR2){
		errorFlag = !errorFlag;
	}
}

void stp_handler(int signo) {

	if(signo == SIGTSTP){
		exitFlag = !exitFlag;	
	}
	
}

	
int main (void) {
	int i;
	/*struct timeval tvstart; 
	struct timeval tvend; 
	struct timeval tvduration; 
	unsigned int duration; 
	time_t curtime;*/
	pthread_t thread[CHILDREN]; 
	char buffer[30]; 

	/*gettimeofday(&tvstart, NULL);

	curtime=tvstart.tv_sec;
	strftime(buffer,30,"%m-%d-%Y  %T.",localtime(&curtime));*/

	srand(time(NULL));
	
	for(i=0; i<CHILDREN; i++) {
		if (pthread_create(&thread[i], NULL, escritor, NULL) == -1) {
			printf("Failed to intialize thread\n");
			return -1;
		}
	}
	
	if((signal(SIGUSR1, usr1_handler))==SIG_ERR){
		printf("Could not catch the SIGUSR1 signal ! \n");
	}
	if((signal(SIGUSR2, usr2_handler))==SIG_ERR){
		printf("Could not catch the SIGUSR2 signal ! \n")
	}
	if((signal(SIGTSTP, stp_handler))==SIG_ERR){
		printf("Could not catch the SIGTSTP signal ! \n")
	
	for(i=0; i<CHILDREN; i++) {
		if (pthread_join(thread[i], NULL) == -1) {
			printf("Thread failed to join the main\n");
			return -1;
		}
	}
	
  	/*gettimeofday(&tvend, NULL); 
  	curtime=tvend.tv_sec;
  	strftime(buffer,30,"%m-%d-%Y  %T.",localtime(&curtime));
  	tvduration.tv_sec = tvend.tv_sec - tvstart.tv_sec;
  	tvduration.tv_usec = tvend.tv_usec - tvstart.tv_usec;
  	duration = tvduration.tv_sec * 1000000 + tvduration.tv_usec;
  	printf("Execution Time  : %d µs\n", duration);*/

	return 0;
}

//SIGUSR1 = inverte uso de trincos
//SIGRUSR2 = inverte escrita com erros, ou seja escreve uma linha diferente no inicio da string, de 2 em 2 linhas
//SIGSTP = exitCommand = 0;

void* escritor(void* arg) {
	int j, fileid, rdmstr, rdmfile;
	

		//for (i=0; i<(CICLE/CHILDREN); i++) {
		while(exitFlag == 0) {
			srand(time(NULL));
			rdmstr = rand()%MAXLETTER;
			rdmfile = rand()%5;
			char str[LENGTH + 1];
			
		

			fileid = open(files[rdmfile], O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IROTH);
			if(lockFlag){
				if(flock(fileid, LOCK_EX)==-1){
					printf("Write lock did not succeed\n");
				}
			}
			
			strcpy(str, strs[rdmstr]);
			str[rand()%(LENGTH-1)] = 'k' + rand()%(MAXLETTER); 
				
		
				
			for(j=0; j<NSTR; j++){
				//creating error
				if (errorFlag && (j%2)) {
					//creating errors
					//strcpy(str, strs[rdmstr]);
					//str[rand()%(LENGTH-1)] = 'j' + rand()%(MAXLETTER); 
					if(write(fileid, str, sizeof(char)*LENGTH) == -1) { 
						printf("Could not write file %s\n",files[rdmfile]);
					}
				}
				else {
					if(write(fileid, strs[rdmstr], sizeof(char)*LENGTH) == -1){
						printf("Could not write file %s\n",files[rdmfile]);
					}
				}
			}
				
				
			if(lockFlag) {
				if(flock(fileid, LOCK_UN)==-1){
					printf("Could not unlock the write access to the file %s\n", files[rdmfile]);
				}
			}
			
			if(close(fileid)==-1){
				printf("Could not close the file %s\n", files[rdmfile]);
			}
	
		}
		return NULL;
}
