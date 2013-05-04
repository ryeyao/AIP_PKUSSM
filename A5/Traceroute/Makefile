CPP = g++
CFLAGS = -c
#CXXFLAGS = -Wall -W
OUTPUT = traceroute
all: $(OUTPUT)

traceroute: Pakiet.o icmp.o sockwrap.o main.o
	$(CXX) $(CXXFLAGS) main.o Pakiet.o sockwrap.o icmp.o -o $(OUTPUT)

main.o: main.cc
	$(CXX) $(CFLAGS) main.cc 
icmp.o: icmp.cc
	$(CXX) $(CFLAGS) icmp.cc icmp.h 
sockwrap.o: sockwrap.cc
	$(CXX) $(CFLAGS) sockwrap.cc sockwrap.h 
Pakiet.o: Pakiet.cc
	$(CXX) $(CFLAGS) Pakiet.cc Pakiet.h 

clean:
	rm -rf *.o $(OUTPUT) *.pch
