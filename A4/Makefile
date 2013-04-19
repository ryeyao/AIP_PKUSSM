CC=gcc
CPP=g++

CFLAGS=-g -Wall

LIBSDIR=libav/lib
LIBS=-pthread -L$(LIBSDIR) -lavformat -lavcodec -lavutil -lasound
#LIBS=-pthread -L/usr/local/libav/lib -lavutil -lasound
INCLUDES= -I. -Ilibav/include

TARGETS=mp3player

MP3PLAYER_H=mp3player.h
MP3PLAYER_C=mp3player.c
MP3PLAYER_O=mp3player.o

all: $(TARGETS)

mp3player: $(MP3PLAYER_O) 
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(LIBS)

mp3player.o:  $(MP3PLAYER_C) $(MP3PLAYER_H)
	$(CC) $(CFLAGS) $(LIBS) $(INCLUDES) -c $(MP3PLAYER_C) 

.PHONY:clean
clean:
	rm -f $(TARGETS)
	rm *.o *.out
