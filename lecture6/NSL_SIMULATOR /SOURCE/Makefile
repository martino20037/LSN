CC = g++
# Changed c++11 to c++17 and kept the Homebrew path
CFLAGS = -O3 -std=c++17 -I/opt/homebrew/include
AFLAGS = -L/opt/homebrew/lib -larmadillo
all: simulator.exe

# The Linking Step (Uses AFLAGS)
simulator.exe : system.o NSL_SIMULATOR.o particle.o random.o
	$(CC) system.o particle.o random.o NSL_SIMULATOR.o -o simulator.exe $(AFLAGS)

# The Compilation Steps (Use CFLAGS)
NSL_SIMULATOR.o : NSL_SIMULATOR.cpp system.h particle.h
	$(CC) -c NSL_SIMULATOR.cpp -o NSL_SIMULATOR.o $(CFLAGS)

system.o : system.cpp system.h random.h particle.h
	$(CC) -c system.cpp -o system.o $(CFLAGS)

particle.o : particle.cpp particle.h
	$(CC) -c particle.cpp -o particle.o $(CFLAGS)

random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)

clean :
	rm *.o simulator.exe

remove:
	rm -f ../OUTPUT/*.*
	rm -f ../OUTPUT/CONFIG/*.*