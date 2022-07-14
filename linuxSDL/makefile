# default
SDL_CONFIG=sdl2-config
CC=g++ -o opzDisplay

linux: build run

build:
	$(CC) -Wall  opzDisplay.cpp rtmidi/RtMidi.cpp -fopenmp -Wno-narrowing `$(SDL_CONFIG) --cflags --libs` -D__LINUX_ALSA__ -lasound -lpthread

run:
	./opzDisplay
