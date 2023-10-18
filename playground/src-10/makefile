CC = gcc
RM = rm -rf
CFLAGS = -std=c99 -c -g -O0 -MD -Wall
LDFLAGS = 
SRC = $(wildcard *.c)
OBJ = $(addsuffix .o, $(basename $(SRC)))
TARGET = test

all: $(TARGET)

$(TARGET): $(OBJ)
	@$(CC) $(LDFLAGS) $^ -o $@
	@echo 'Done!'

%.o: %.c
	@$(CC) $(CFLAGS) $< -o $@

include $(wildcard *.d)

clean:
	@$(RM) *.o *.d
	@$(RM) $(TARGET)
	@echo 'Cleaning ... Done!'
