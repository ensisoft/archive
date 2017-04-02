

SOURCES = main.cpp \
	ncurses.cpp\
	welcome.cpp\
	gameview.cpp\

HEADERS = buffer.h \
	formatter.h \
	game.h \
	keys.h \
	terminal.h \
	view.h \
	welcome.h \
	gameview.h \
	buffer.h\
	cursor.h\

OBJS=$(SOURCES:.cpp=.o)


CC = g++
CFLAGS = -Wall -std=c++11 -O0 -g
LIBS = -lcurses

%.o: %.cpp 
	$(CC) $(CFLAGS) -c -o $@ $< 

mastermind: $(OBJS) $(HEADERS)
	$(CC) -o mastermind $(OBJS) $(LIBS)

unit-test: $(OBJS) $(HEADERS) unit_test/unit_test.cpp
	$(CC) -o unit-test unit_test/unit_test.cpp $(CFLAGS) $(LIBS)

clean:
	rm -f *.o
	rm -f mastermind
	rm -f unit-test

all: mastermind unit-test