# Makefile
CC = gcc -O6 -Wall -g
L = -lm -lilist -listring
TAR = test
OBJS = main.o ihash.o
SHAD = -fPIC -shared
LIB = libihash.so

all : $(TAR) lib
$(TAR): $(OBJS)
	  $(CC) -o $@ $(OBJS) $(L)
clean:
	  rm -f $(OBJS) $(TAR) $(LIB) *~ *.bak

.c.o:	  $(CC) -c $<

lib:
	$(CC) $(SHAD) -o $(LIB) ihash.c $(L)

main.o: ihash.h
ihash.o: ihash.h
