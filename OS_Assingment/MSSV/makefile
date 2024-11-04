#MAKEFILE for easy compilation

CC = gcc
CFLAGS = -Wall -g -std=c99 -pedantic
OBJ = main.o cmdlValidation.o fileIO.o threadding.o finalOutput.o
EXEC = mssv

$(EXEC) : $(OBJ)
	$(CC) $(OBJ) -o $(EXEC)

main.o : main.c ThreadDataStruct.h
	$(CC) $(CFLAGS) main.c -c

cmdlValidation.o : cmdlValidation.c cmdlValidation.h
	$(CC) $(CFLAGS) cmdlValidation.c -c

fileIO.o : fileIO.c fileIO.h
	$(CC) $(CFLAGS) fileIO.c -c

threadding.o : threadding.c threadding.h
	$(CC) $(CFLAGS) threadding.c -c

finalOutput.o : finalOutput.c finalOutput.h
	$(CC) $(CFLAGS) finalOutput.c -c
	
clean :
	rm -f $(OBJ) $(EXEC)