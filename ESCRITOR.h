#ifndef ESCRITOR_H_
#define ESCRITOR_H_

#define LENGTH 10
#define NSTR 1024
#define CICLE 5120
#define MAXLETTER 10
#define CHILDREN 10

void* escritor(void*);

int exitFlag = 0;
int lockFlag = 1;
int errorFlag = 0;



char strs[MAXLETTER][LENGTH+1] = {"aaaaaaaaa\n", "bbbbbbbbb\n", "ccccccccc\n", "ddddddddd\n", "eeeeeeeee\n",
       "fffffffff\n", "ggggggggg\n", "hhhhhhhhh\n", "iiiiiiiii\n", "jjjjjjjjj\n"};

char files[5][13] = {"SO2014-0.txt", "SO2014-1.txt", "SO2014-2.txt", "SO2014-3.txt", "SO2014-4.txt"};


#endif