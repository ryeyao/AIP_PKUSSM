CC=gcc
CPP=g++

CFLAGS=-Wall -g -lboost_regex

WORDSEP_C=EngWordsSeparator.cc
WORDSEP_TEST_C=EngWordsSeparator_regex.cc

WORDSEP_TEST_H=-I/home/rye/Programs/libs/boost_1_53_0/boost 

LIBS=-L/home/rye/Programs/libs/boost_1_53_0/libs 

TARGETS=parse parse_regex

all:$(TARGETS)

parse:
	$(CPP) $(CFLAGS)  -o parse $(WORDSEP_C)
parse_regex:
	$(CPP) -o parse_regex $(WORDSEP_TEST_H) $(LIBS) $(WORDSEP_TEST_C) $(CFLAGS)


clean:
	rm -f $(TARGETS)
