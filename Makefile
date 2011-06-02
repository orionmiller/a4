CC = gcc
CFLAGS = -Wall -g 

GET_SRCS = fslib.c minget.c

all: minget #fs minls minget

#fs: #dependencies
#	$(CC) $(CFLAGS) $ -o minls

minget: $(GET_SRCS)
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
	@rm *~ *.0

allclean:
	@rm *~ *.o minls minget