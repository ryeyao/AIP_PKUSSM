CC=gcc
CPP=g++

CFLAGS=-Wall -g

TARGETS= ls_l ls_tree

all:$(TARGETS)

ls_l: ls_l.c
	$(CC) $(CFLAGS)  -o $@ $^ 
ls_tree: ls_tree.c
	$(CC) $(CFLAGS) -o $@ $^

.PHONY:clean
clean:
	rm -f $(TARGETS)
