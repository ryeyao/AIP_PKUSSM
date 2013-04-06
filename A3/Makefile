CC = gcc
CPP = g++

CFLAGS = -g -Wall

CFLAGS += -lpthread

TARGETS = Server

SERVER_C = Server.cpp
SERVER_H = Server.h

all: $(TARGETS)

Server: $(SERVER_C) $(SERVER_H)
	$(CPP) $(CFLAGS) -o $@ $^

%.o : %.c
$(C) -g -c $<

.PHONY:clean
clean:
	rm -f $(TARGETS)
