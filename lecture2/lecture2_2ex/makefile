CC = g++
CFLAGS = -Wall -O3 --std=c++11

lecture_2_ex2.exe : lecture_2_ex2.o random.o
	$(CC) random.o lecture_2_ex2.o -o lecture_2_ex2.exe
lecture_2_ex2.o : lecture_2_ex2.cpp
	$(CC) -c lecture_2_ex2.cpp -o lecture_2_ex2.o $(CFLAGS)
random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)
clean :
	rm *.o lecture_2_ex2.exe seed.out