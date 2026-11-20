CC = g++
CFLAGS = -Wall -O3 --std=c++11

lecture_1_2.exe : lecture_1_2.o random.o
	$(CC) random.o lecture_1_2.o -o lecture_1_2.exe
lecture_1_2.o : lecture_1_2.cpp
	$(CC) -c lecture_1_2.cpp -o lecture_1_2.o $(CFLAGS)
random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)
clean :
	rm *.o lecture_1_2.exe seed.out