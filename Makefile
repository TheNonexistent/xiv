HEADERS = Include/buffer.h Include/file.h

default: xiv

xim.o: xiv.cpp $(HEADERS)
	g++ -c xiv.cpp

file.o: Include/file.cpp $(HEADERS)
	g++ -c Include/file.cpp

buffer.o: Include/buffer.cpp $(HEADERS)
	g++ -c Include/buffer.cpp

xim: xim.o file.o buffer.o
	g++ xiv.o file.o buffer.o -o xiv -lncurses -Wall

clean:
	-rm -f xiv.o
	-rm -f buffer.o
	-rm -f file.o
	-rm -f xiv
