EXEC=Eshell
SOURCES=shell.c 
OBJECTS=$(SOURCES:.c=.o)
CC=gcc
CFLAGS=-W -Wall -Werror -std=gnu99 -g 
 
.PHONY: default clean
 
default: $(EXEC)

shell.o: shell.c shell.h

$(EXEC): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ 

clean:
	rm -rf $(EXEC) $(OBJECTS) $(SOURCES:.c=.c~) $(SOURCES:.c=.h~) Makefile~
