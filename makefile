all: hencode hdecode

hencode: hencode.o
	g++ -o hencode hencode.o

hdecode: hdecode.o
	g++ -o hdecode hdecode.o

hdecode.o: hdecode.cc
	g++ -c hdecode.cc

hencode.o: hencode.cc
	g++ -c hencode.cc

clean:
	rm -rf hencode.o hdecode.o hencode hdecode