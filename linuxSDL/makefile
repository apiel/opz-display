# default
SDL_CONFIG=sdl2-config
CC=g++ -o opzDisplay

linux: build run

build:
	$(CC) -Wall  opzDisplay.cpp -fopenmp -Wno-narrowing `$(SDL_CONFIG) --cflags --libs`

run:
	./opzDisplay
