# default
SDL_CONFIG=sdl2-config
CC=g++ -o opzDisplay

linux: build run

forward:
	g++ -o midiForward -Wall  midiForward.cpp rtmidi/RtMidi.cpp -D__LINUX_ALSA__ -lasound -lpthread

build:
	$(CC) -Wall  sdlOpzDisplay.cpp rtmidi/RtMidi.cpp -fopenmp -Wno-narrowing `$(SDL_CONFIG) --cflags --libs` -D__LINUX_ALSA__ -lasound -lpthread

run:
	./opzDisplay
