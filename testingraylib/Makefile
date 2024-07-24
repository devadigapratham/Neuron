CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -I/usr/include/raylib -I/usr/include/json-c
LDFLAGS = -lraylib -ljson-c -lm

all: visualize_network

visualize_network: visualize_network.o
	$(CC) -o $@ $^ $(LDFLAGS)

visualize_network.o: visualize_network.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o visualize_network
