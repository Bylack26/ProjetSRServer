.PHONY: all, clean

# Disable implicit rules
.SUFFIXES:

# Keep intermediate files
#.PRECIOUS: %.o

CC = gcc
CFLAGS = -Wall
LDFLAGS =

# Note: -lnsl does not seem to work on Mac OS but will
# probably be necessary on Solaris for linking network-related functions 
#LIBS += -lsocket -lnsl -lrt
LIBS += -lpthread

INCLUDE = csapp.h serverFunction.h
OBJS = csapp.o serverFunction.o
INCLDIR = -I.
CLIENTDEPENDANCE = csapp.o mdir
SERVERDEPENDANCE = csapp.o serverFunction.o
PROGS = server client mdir


all: $(PROGS)

%.o: %.c $(INCLUDE)
	$(CC) $(CFLAGS) $(INCLDIR) -c -o $@ $<
	
%: %.o $(OBJS)
	$(CC) -o $@ $(LDFLAGS) $^ $(LIBS)
#	$(CC) -o $@ $(LDFLAGS) $(LIBS) $^
	
mdir: 
	-mkdir ./DirClient/

clean:
	rm -f $(PROGS) *.o
	-rm -f ./DirClient/*
	-rm -f ./DirClient/.*

client: $(CLIENTDEPENDANCE)
	$(CC) -o $@ $(LDFLAGS) $^ $(LIBS)

server: $(SERVERDEPENDANCE)
	$(CC) -o $@ $(LDFLAGS) $^ $(LIBS)
