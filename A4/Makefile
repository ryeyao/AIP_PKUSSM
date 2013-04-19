CC = gcc
CPP = g++

CFLAGS = -g -Wall
#CFLAGS += -D__STDC_CONSTANT_MACROS

LIBS = -pthread -L./libav/libavcodec/ -lavcodec -L./libav/libavformat/ -lavformat -L./libav/libavutil -lavutil
#INCLUDES = -I./include/libavcodec -I./include/libavformat -I./include/libavutil
INCLUDES = -I./libav -I .

TARGETS = mp3player

MP3PLAYER_H = mp3player.h
MP3PLAYER_C = mp3player.c
MP3PLAYER_O = mp3player.o

all: $(TARGETS)
	$(shell export LD_LIBRARY_PATH=libav/libavformat:libav/libavcodec:libav/libavutil:$LD_LIBRARY_PATH)

mp3player: $(MP3PLAYER_O) 
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(LIBS)

mp3player.o:  $(MP3PLAYER_C) $(MP3PLAYER_H)
	$(CC) $(CFLAGS) $(LIBS) $(INCLUDES) -c $(MP3PLAYER_C) 

.PHONY:clean
clean:
	rm -f $(TARGETS)
	rm *.o *.out
