CC = g++
CFLAGS = -Wall -O3 --std=c++11

lecture_1_3.exe : lecture_1_3.o random.o
	$(CC) random.o lecture_1_3.o -o lecture_1_3.exe
lecture_1_3.o : lecture_1_3.cpp
	$(CC) -c lecture_1_3.cpp -o lecture_1_3.o $(CFLAGS)
random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)
clean :
	rm *.o lecture_1_3.exe seed.out