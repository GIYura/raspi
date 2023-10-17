RM = rm -rf
CC = gcc

all: blink 

blink: led-test.o
	@$(CC) $^ -o $@
	@echo 'Build done'

%.o: %.c
	@$(CC) -c $^ -o $@

clean:
	@$(RM) *.o
	@$(RM) blink
	@echo 'Cleaning ... done'
