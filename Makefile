CC = gcc
CFLAGS = -Wall -g 


all: fs minls minget

fs: #dependencies
	$(CC) $(CFLAGS) $^ -o minls

minget: #dependencies
	$(CC) $(CFLAGS) $^ -o minget

minls: #dependencies
	$(CC) $(CFLAGS) $^ -o minls

handin:
	@echo "add handin script"

nicoTest:
	@echo "add nico test bench"

test:
	@echo "add personal test bench"

clean:
	@rm *~ *.0

allclean:
	@rm *~ *.o minls minget