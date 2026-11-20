CC = mpicxx
CFLAGS = -Wall -O3 --std=c++11

lecture10.exe : lecture10.o random.o
	$(CC) random.o lecture10.o -o lecture10.exe

lecture10.o : lecture10.cpp city.h individual.h population.h random.h
	$(CC) -c lecture10.cpp -o lecture10.o $(CFLAGS)

random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)

clean :
	rm -f *.o lecture10.exe seed.out