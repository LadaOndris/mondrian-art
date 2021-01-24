

CC = gcc
CFLAGS = -Wall -Wextra -O2 -pedantic
EXECUTABLE = art

.PHONY: clean

art: main.o bitmap.o
	$(CC) $(CFLAGS) $^ -o $(EXECUTABLE)

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@ 

clean:
	@rm -f *.o
	@rm -f $(EXECUTABLE)
	

