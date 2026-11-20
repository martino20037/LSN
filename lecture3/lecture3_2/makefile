CC = g++
CFLAGS = -Wall -O3 --std=c++11

lecture3_2.exe : lecture3_2.o random.o
	$(CC) random.o lecture3_2.o -o lecture3_2.exe
lecture3_2.o : lecture3_2.cpp
	$(CC) -c lecture3_2.cpp -o lecture3_2.o $(CFLAGS)
random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)
clean :
	rm *.o lecture3_2.exe seed.out