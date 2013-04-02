CC=gcc
CPP=g++

CFLAGS=-Wall -g

TARGETS= ls_l

all:$(TARGETS)

ls_l: ls_l.c
	$(CC) $(CFLAGS)  -o $@ $^ 

.PHONY:clean
clean:
	rm -f $(TARGETS)
