CC = g++
CFLAGS = -Wall -O3 --std=c++11

lecture8.exe : lecture8.o random.o
	$(CC) random.o lecture8.o -o lecture8.exe
lecture8.o : lecture8.cpp
	$(CC) -c lecture8.cpp -o lecture8.o $(CFLAGS)
random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)
clean :
	rm *.o lecture8.exe seed.out