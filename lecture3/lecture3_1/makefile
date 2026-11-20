CC = g++
CFLAGS = -Wall -O3 --std=c++11

lecture3.exe : lecture3.o random.o
	$(CC) random.o lecture3.o -o lecture3.exe
lecture3.o : lecture3.cpp
	$(CC) -c lecture3.cpp -o lecture3.o $(CFLAGS)
random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)
clean :
	rm *.o lecture3.exe seed.out