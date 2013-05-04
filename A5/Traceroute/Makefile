CPP = g++
CFLAGS = -c
#CXXFLAGS = -Wall -W
OUTPUT = traceroute
all: $(OUTPUT)

traceroute: Packet.o icmp.o sockwrap.o main.o
	$(CXX) $(CXXFLAGS) main.o Packet.o sockwrap.o icmp.o -o $(OUTPUT)

main.o: main.cc
	$(CXX) $(CFLAGS) main.cc 
icmp.o: icmp.cc
	$(CXX) $(CFLAGS) icmp.cc icmp.h 
sockwrap.o: sockwrap.cc
	$(CXX) $(CFLAGS) sockwrap.cc sockwrap.h 
Packet.o: Packet.cc
	$(CXX) $(CFLAGS) Packet.cc Packet.h 

clean:
	rm -rf *.o $(OUTPUT)
deepclean:
	rm -rf *.o $(OUTPUT) *.gch
