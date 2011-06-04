CC = gcc
CFLAGS = -Wall -g 



GET_SRCS = fslib.c minget.c toklib.c
GET_HDRS = fslib.h inode.h toklib.h

LS_SRCS = fslib.c minls.c toklib.c
LS_HDRS = fslib.h inode.h toklib.h

#all: minget #fs minls minget

#fs: #dependencies
#	$(CC) $(CFLAGS) $ -o minls

minget: $(GET_SRCS) $(GET_HDRS)
	$(CC) $(CFLAGS) $(GET_SRCS) -o minget

minls: $(LS_SRCS) $(LS_HDRS)
	$(CC) $(CFLAGS) $(LS_SRCS) -o minls

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
