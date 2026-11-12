CC = g++
CFLAGS = -Wall -O3 --std=c++11

lecture_1.exe : lecture_1.o random.o
	$(CC) random.o lecture_1.o -o lecture_1.exe
lecture_1.o : lecture_1.cpp
	$(CC) -c lecture_1.cpp -o lecture_1.o $(CFLAGS)
random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)
clean :
	rm *.o lecture_1.exe seed.out
