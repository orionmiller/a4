# Authors: Geoffrey Lawson, Orion Miller
# Class: CPE 453
# Assignment 4

CC = gcc
CFLAGS = -Wall -g 

GET_SRCS = fslib.c minget.c toklib.c verbose.c
GET_HDRS = fslib.h inode.h toklib.h verbose.h

LS_SRCS = fslib.c minls.c toklib.c verbose.c
LS_HDRS = fslib.h inode.h toklib.h verbose.h


#all: minget #fs minls minget

#fs: #dependencies
#	$(CC) $(CFLAGS) $ -o minls

all: minget minls

minget: $(GET_SRCS) $(GET_HDRS)
	$(CC) $(CFLAGS) $(GET_SRCS) -o minget

minls: $(LS_SRCS) $(LS_HDRS)
	$(CC) $(CFLAGS) $(LS_SRCS) -o minls

handin: allclean
	handin pn-cs453 Asgn4 *

clean: 
	rm -f *~ *.o

allclean:
	rm -f *~ *.o *#*#* minls minget
