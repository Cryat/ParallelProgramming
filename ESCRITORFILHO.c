#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/file.h>
#include "ESCRITOR.h"

int main() {
	int i, j, fileid, rdmstr, rdmfile;
	

		for (i=0; i<(CICLE/CHILDREN); i++) {
		
			rdmstr = rand()%MAXLETTER;
			rdmfile = rand()%5;
			
			fileid = open(files[rdmfile], O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IROTH);
			if(flock(fileid, LOCK_EX)==-1){
				printf("Write lock did not succeed\n");
				exit(-1);
			}

			for(j=0; j<NSTR; j++)
				if(write(fileid, strs[rdmstr], sizeof(char)*LENGTH) == -1){
					printf("Could not write file %s\n",files[rdmfile]);
				}
			
			if(flock(fileid, LOCK_UN)==-1){
				printf("Could not unlock the read access to the file %s\n", files[rdmfile]);
				exit(-1);
			}
			if(close(fileid)==-1){
				printf("Could not close the file %s\n", files[rdmfile]);
				exit(-1);
			}
	
		}
	return 0;
}
