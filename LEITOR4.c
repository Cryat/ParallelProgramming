#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>
#include <sys/file.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include "LEITOR.h"

int main() {
	pthread_t thread[CHILDREN];
	void *value;
	int i;
	char tmpBuffer[14];
	
	sem_init(&w8input, 0, 0);
	sem_init(&w8output, 0, MAX);
	pthread_mutex_init(&mutex, NULL);

	srand(time(NULL));

	for(i=0; i<CHILDREN; i++) {
		if (pthread_create(&thread[i], NULL, leitor, NULL) == -1) {
			printf("Failed to intialize thread\n");
			return -1;
		}
		
	}
	
	
	while(exitFlag != 0){
		int size = read(STDFD, tmpBuffer, sizeof(char)*13);
		
		if (size == -1)
			printf("Reader input did not succeed\n");
		else if ((size > 0) && (tmpBuffer[size-1] == '\n' || tmpBuffer[size-1] == ' '))
			tmpBuffer[size-1] = '\0'; //ignorable \n
		else if (size == 0)
			exitFlag = !exitFlag;
		else
			tmpBuffer[size] = '\0'; //max size
			
		
		if ((tmpBuffer[0] != '\0') && (size > 0))	//ignores empty strings
			insertInBuffer(tmpBuffer);
	}
	
	//unblocks all threads
	for (i=0; i<CHILDREN; i++) {
		if (sem_post(&w8input) == -1)
			printf("Reader sempahore has failed \n");
	}
	
	for(i=0; i<CHILDREN; i++) {
		
		if (pthread_join(thread[i], NULL) == -1) {
			printf("Failed to finalize thread\n");
			return -1;
		}
		
	}
	
	return 0;
	
}



void * leitor(void * arg) {

	int fileid;
	long int value;
	char file[LENGTH+1], fileBuffer[13];
	char compstr[LENGTH+1];
	while(exitFlag != 0){
		
		removeFromBuffer(fileBuffer);
		
		if (exitFlag == 0) break;
	
		fileid = open(fileBuffer, O_RDONLY);
		if (fileid == -1){
			printf("File %s does not exist or failed to open\n", fileBuffer);
			continue;
		}
		
		if(flock(fileid, LOCK_SH || LOCK_NB) == -1){
			printf("Read lock did not succeed\n");
			continue;
		}
	
		if(read(fileid, file, sizeof(char)*LENGTH)==-1){
			printf("Could not read file %s\n", fileBuffer);
			continue;
	  	}
	
		file[LENGTH] = '\0';  /* Transforms the characters read to the buffer on the read() function  into a String with a '\0' */
		
		if (checkStr(file) == 0) {
			strcpy(compstr, file);
			if(checkAllStr(fileid, compstr)==0) {
				printf("File %s is according to format\n", fileBuffer);
			}
			else {
				printf("File %s not according to format\n", fileBuffer);
			}
		}
		else {
			printf("File %s has invalid first string\n", fileBuffer);
		}
		
		
		if(flock(fileid, LOCK_UN)==-1){
			printf("Could not unlock the read access to the file %s\n", fileBuffer);
		}
		if(close(fileid)==-1){
			printf("Could not close the file %s\n", fileBuffer);
		}
	}
	pthread_exit(NULL);
}

/* char * ---> int */
/* Checks if the first string in the file is valid returning 0. Otherwise it returns -1 */
int checkStr(char * file) { 

	int i;

	for(i=0; i<MAXLETTER; i++) {
		if (strcmp(strs[i], file) == 0) 
			return 0;
	}
	return -1;
}

/* int X char * ---> int */
/* Checks if every string in the file are equal returning 0. Otherwise it returns -1 */
int checkAllStr(int fileid, char * compstr) {
	
	char file[LENGTH+1];
	int i;

	for(i=1; i<NSTR; i++) {
		int bytesRead = read(fileid, file, sizeof(char)*LENGTH);
		if (bytesRead == -1) {
			printf("Could not read file\n");
			return -1;
		}
		else if (bytesRead == 0){
			return -1;
		}
		file[LENGTH] = '\0';		
		if (strcmp(compstr, file) != 0){
			return -1;
		}
	}
	return read(fileid, file, sizeof(char)*LENGTH);
}

void insertInBuffer(char * item){
	if (sem_wait(&w8output) == -1) {
		printf("Insert: semaphore has failed\n");
		return;
	}
	if (pthread_mutex_lock(&mutex) == -1) {
		printf("Insert: mutex lock has failed\n");
		return;
	}
	
	strcpy(sharedBuffer[inputIndex], item);
	inputIndex = (inputIndex+1)%MAX ;
	
	if (sem_post(&w8input) == -1) {
		printf("Insert: semaphore has failed\n");
		return;
	}
	if (pthread_mutex_unlock(&mutex) == -1) {
		printf("Insert: mutex unlock has failed\n");
		return;
	}
}
	
void removeFromBuffer(char* threadBuffer){
	if (sem_wait(&w8input) == -1) {
		printf("Remove: semaphore has failed\n");
		return;
	}
	if (pthread_mutex_lock(&mutex) == -1) {
		printf("Remove: mutex lock has failed\n");
		return;
	}
	
	strcpy(threadBuffer , sharedBuffer[outputIndex]);
	outputIndex = (outputIndex+1)%MAX ;

	if (sem_post(&w8output) == -1) {
		printf("Remove: semaphore has failed\n");
		return;
	}
	
	if (pthread_mutex_unlock(&mutex) == -1) {
		printf("Remove: mutex unlock has failed\n");
		return;
	}
}

