All: ESCRITOR ESCRITORFILHO LEITOR4

LEITOR4.o: LEITOR4.c LEITOR.h
	gcc -pthread -Wall -g -c LEITOR4.c

LEITOR4: LEITOR4.o
	gcc -pthread -Wall -o LEITOR4 LEITOR4.o

ESCRITORFILHO.o: ESCRITOR.h ESCRITORFILHO.c
	gcc -Wall -g -c ESCRITORFILHO.c

ESCRITORFILHO: ESCRITORFILHO.o
	gcc -Wall -o ESCRITORFILHO ESCRITORFILHO.o

ESCRITOR: ESCRITOR.o
	gcc -Wall -o ESCRITOR ESCRITOR.o

ESCRITOR.o: ESCRITOR.h ESCRITOR.c
	gcc -Wall -g -c ESCRITOR.c
