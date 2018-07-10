HEADERS = Include/buffer.h Include/file.h

default: xim

xim.o: xim.cpp $(HEADERS)
	g++ -c xim.cpp

file.o: Include/file.cpp $(HEADERS)
	g++ -c Include/file.cpp

buffer.o: Include/buffer.cpp $(HEADERS)
	g++ -c Include/buffer.cpp

xim: xim.o file.o buffer.o
	g++ xim.o file.o buffer.o -o xim -lncurses -Wall

clean:
	-rm -f xim.o
	-rm -f buffer.o
	-rm -f file.o
	-rm -f xim
