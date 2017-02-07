CC = g++
CFLAGS = -g -O3

all: ui gen ai

ui: ui.o obj.o resources.o group.o
	$(CC) $(CFLAGS) $^ -o $@ -lsfml-graphics -lsfml-window -lsfml-system
	
gen: gen.cpp
	$(CC) $(CFLAGS) $^ -o $@
	
ai: ai.cpp
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) -c $<

clean:
	rm ui ai gen *.o