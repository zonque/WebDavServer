all: main

clean:
	rm -f *.o *.gch main

BASEDIR=../..
CPPFLAGS = \
	-Wall -Werror \
	-I. -I$(BASEDIR)/include \
	 `pkg-config --cflags libsoup-2.4` \
	 `pkg-config --cflags tinyxml2`
LDFLAGS = \
	`pkg-config --libs libsoup-2.4` \
	`pkg-config --libs tinyxml2`

server.o: $(BASEDIR)/src/server.cpp $(BASEDIR)/include/server.h
	g++ $(CPPFLAGS) -c -o $@ $<

request.o: $(BASEDIR)/src/request.cpp $(BASEDIR)/include/request.h
	g++ $(CPPFLAGS) -c -o $@ $<

response.o: $(BASEDIR)/src/response.cpp $(BASEDIR)/include/response.h
	g++ $(CPPFLAGS) -c -o $@ $<

main: main.o server.o request.o response.o
	g++ $(CPPFLAGS) $(LDFLAGS) -o $@ $^

