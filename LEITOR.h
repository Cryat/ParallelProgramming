#ifndef LEITOR_H_
#define LEITOR_H_


#define LENGTH 10
#define NSTR 1024
#define MAXLETTER 10
#define CHILDREN 3 
#define MAX 10
#define STDFD 0
#define EXIT 1

char strs[MAXLETTER][LENGTH+1] = {"aaaaaaaaa\n", "bbbbbbbbb\n", "ccccccccc\n", "ddddddddd\n", "eeeeeeeee\n",
						 "fffffffff\n", "ggggggggg\n", "hhhhhhhhh\n", "iiiiiiiii\n", "jjjjjjjjj\n"};

char files[5][13] = {"SO2014-0.txt", "SO2014-1.txt", "SO2014-2.txt", "SO2014-3.txt", "SO2014-4.txt"};

char sharedBuffer[MAX][13];

int inputIndex = 0;
int outputIndex= 0;
int exitFlag = 1;

sem_t w8input;
sem_t w8output;
pthread_mutex_t mutex;

typedef struct instructions{
	char *filename;
	int startline;
	int nlines;
	char *output;
	int bytesleft;
}instructions; 


void* leitor(void *);
int checkStr(char * file);
int checkAllStrThreads(int fileid, char * compstr, int nlines);
int checkAllStr(int fileid, char * compstr);
void insertInBuffer(char * item);
void removeFromBuffer(char* item);

#endif
