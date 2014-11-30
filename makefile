#------------------------------------------------------------------------------

PROGRAM=graphphasetrans
EXENAME=phasetrans
CC=g++ -g

#------------------------------------------------------------------------------


all:  $(PROGRAM)

graphphasetrans:  main.o dfs.o 
	$(CC) main.o dfs.o -o $(EXENAME)

main.o: main.cpp globals.h
	$(CC) -c main.cpp 

dfs.o: dfs.cpp globals.h
	$(CC) -c dfs.cpp
clean:
	rm -f *.o *~  $(PROGRAM)
