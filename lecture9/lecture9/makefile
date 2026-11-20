CC = g++
CFLAGS = -Wall -O3 --std=c++11

lecture9.exe : lecture9.o random.o
	$(CC) random.o lecture9.o -o lecture9.exe

lecture9.o : lecture9.cpp city.h individual.h population.h random.h
	$(CC) -c lecture9.cpp -o lecture9.o $(CFLAGS)

random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)

clean :
	rm -f *.o lecture9.exe seed.out