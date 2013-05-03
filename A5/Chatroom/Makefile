CC = gcc
CPP = g++

CFLAGS = -g -Wall -std=c++0x #-MM

CFLAGS += -pthread

CFLAGS += $(INCS)

INCS = -I./include

DEFINES = -DMSG_STRATEGY_PIPE
CFLAGS += $(DEFINES)

TARGETS = bin/Server bin/Client

SERVER_C = src/Server.cc
SERVER_H = include/Server.h
SERVER_O = Server.o

CLIENT_C = src/Client.cc
CLIENT_H = include/Client.h
CLIENT_O = Client.o

HANDLERS_C = src/Handlers.cc
HANDLERS_H = include/Handlers.h
HANDLERS_O = Handlers.o

MESSAGEPOLL_H = include/MessagePoll.h
MESSAGEPOLL_O = MessageQueue.o MessagePipe.o MessageSharedMemory.o
MESSAGEPIPE_H = include/MessagePipe.h
MESSAGEPIPE_C = src/MessagePipe.cc
MESSAGEQUEUE_H = include/MessageQueue.h
MESSAGEQUEUE_C = src/MessageQueue.cc
MESSAGESHAREDMEMORY_H = include/MessageSharedMemory.h
MESSAGESHAREDMEMORY_C = src/MessageSharedMemory.cc

all: $(TARGETS)

bin/Server: $(SERVER_O) $(MESSAGEPOLL_O) $(HANDLERS_O) 
	$(CPP) $(CFLAGS) -o $@ $^

bin/Client: $(CLIENT_O)
	$(CPP) $(CFLAGS) -o $@ $^

Server.o: $(SERVER_C) $(SERVER_H) $(HANDLERS_H) $(MESSAGEPOLL_H)
	$(CPP) $(CFLAGS) -c $(SERVER_C)

Handlers.o: $(HANDLERS_C) $(HANDLERS_H) $(SERVER_H)
	$(CPP) $(CFLAGS) -c $(HANDLERS_C)

MessageQueue.o: $(MESSAGEQUEUE_C) $(MESSAGEQUEUE_H) $(MESSAGEPOLL_H)
	$(CPP) $(CFLAGS) -c $(MESSAGEQUEUE_C)

MessagePipe.o: $(MESSAGEPIPE_C) $(MESSAGEPIPE_H) $(MESSAGEPOLL_H)
	$(CPP) $(CFLAGS) -c $(MESSAGEPIPE_C)
	
MessageSharedMemory.o: $(MESSAGESHAREDMEMORY_C) $(MESSAGESHAREDMEMORY_H) $(MESSAGEPOLL_H)
	$(CPP) $(CFLAGS) -c $(MESSAGESHAREDMEMORY_C)

Client.o: $(CLIENT_C) $(CLIENT_H) 
	$(CPP) $(CFLAGS) -c $(CLIENT_C)

.PHONY:clean
clean:
	rm -f $(TARGETS)
	rm -rf bin/*
	rm -f test
	rm -f src/*.o src/*.out *.o
