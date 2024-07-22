CC = gcc
CFLAGS = -Wall -Wextra
LIBS = -lSDL2
INCLUDES = -I/usr/include/SDL2/
SRCS = neuralnet.c
OBJS = $(SRCS:.c=.o)
MAIN = neuralnet


$(MAIN) : $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LIBS)
.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) *.o *~ $(MAIN)
