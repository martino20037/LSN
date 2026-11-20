CC = g++
CFLAGS = -Wall -O3 --std=c++11

lecture5_1.exe : lecture5_1.o random.o
	$(CC) random.o lecture5_1.o -o lecture5_1.exe
lecture5_1.o : lecture5_1.cpp
	$(CC) -c lecture5_1.cpp -o lecture5_1.o $(CFLAGS)
random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)
clean :
	rm *.o lecture5_1.exe seed.out