CC = gcc
CPP = g++

CFLAGS = -g -Wall

CFLAGS += -pthread

TARGETS = Server Client

SERVER_C = $(FILESERVER_C) $(HANDLERS_C)
SERVER_H = $(FILESERVER_H) $(HANDLERS_H)
SERVER_O = $(FILESERVER_O) $(HANDLERS_O)

CLIENT_C = $(FILECLIENT_C) 
CLIENT_H = $(FILECLIENT_H)
CLIENT_O = $(FILECLIENT_O)

FILESERVER_C = FileServer.cc
FILESERVER_H = FileServer.h
FILESERVER_O = FileServer.o

HANDLERS_C = Handlers.cc
HANDLERS_H = Handlers.h
HANDLERS_O = Handlers.o

FILECLIENT_C = FileClient.cc
FILECLIENT_H = FileClient.h
FILECLIENT_O = FileClient.o

all: $(TARGETS)
	cp Server tests/Server
	cp Client tests/Client


Server: $(SERVER_O)
	$(CPP) $(CFLAGS) -o $@ $^

Client: $(CLIENT_O)
	$(CPP) $(CFLAGS) -o $@ $^

FileServer.o:  $(FLIESERVER_C) $(FILESERVER_H) $(HANDLERS_H) 
	$(CPP) $(CFLAGS) -c $(FILESERVER_C) 

Handlers.o:  $(HANDLERS_C) $(HANDLERS_H) $(FILESERVER_H)
	$(CPP) $(CFLAGS) -c $(HANDLERS_C)

FileClient.o: $(FILECLIENT_C) $(FILECLIENT_H) 
	$(CPP) $(CFLAGS) -c $(FILECLIENT_C)

test: test.cc
	$(CPP) $(CFLAGS) -o $@ $^

.PHONY:clean
clean:
	rm -f $(TARGETS)
	rm -f test
	rm *.o *.out
