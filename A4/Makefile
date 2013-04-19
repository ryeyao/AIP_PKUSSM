CC = gcc
CPP = g++

CFLAGS = -g -Wall
#CFLAGS += -D__STDC_CONSTANT_MACROS

LIBS = -pthread -L./lib/libavcodec/ -lavcodec -L./lib/libavformat/ -lavformat -L./lib/libavutil -lavutil -lasound
INCLUDES = -I./include -I . -I/usr/include

TARGETS = mp3player

MP3PLAYER_H = mp3player.h
MP3PLAYER_C = mp3player.c
MP3PLAYER_O = mp3player.o

all: $(TARGETS)
	$(shell export LD_LIBRARY_PATH=lib/libavformat:lib/libavcodec:lib/libavutil:$LD_LIBRARY_PATH)

mp3player: $(MP3PLAYER_O) 
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(LIBS)

mp3player.o:  $(MP3PLAYER_C) $(MP3PLAYER_H)
	$(CC) $(CFLAGS) $(LIBS) $(INCLUDES) -c $(MP3PLAYER_C) 

.PHONY:clean
clean:
	rm -f $(TARGETS)
	rm *.o *.out
