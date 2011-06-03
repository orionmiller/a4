CC = gcc
CFLAGS = -Wall -g 

GET_SRCS = fslib.c minget.c
GET_HDRS = fslib.h inode.h

all: minget #fs minls minget

#fs: #dependencies
#	$(CC) $(CFLAGS) $ -o minls

minget: $(GET_SRCS) $(GET_HDRS)
	$(CC) $(CFLAGS) $(GET_SRCS) -o minget

#minls: #dependencies
#	@echo "no minls yet"
#	CC) $(CFLAGS) -o minls

handin:
	@echo "add handin script"

#nicoTest:
#	@echo "add nico test bench"

#test:
#	@echo "add personal test bench"

clean: 
	@rm *~ *.o

allclean:
	@rm *~ *.o minls minget