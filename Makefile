all: gcsample

gc.o: src/gc.cpp include/gc.h
	g++ --std=c++17 -c src/gc.cpp -I./include

gcsample.o: gcsample.cpp
	g++ --std=c++17 -c gcsample.cpp -I./include

gcsample: gc.o gcsample.o
	g++ -o gcsample gc.o gcsample.o

clean:
	rm -f gc.o gcsample.o gcsample
