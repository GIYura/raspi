CC = gcc
STANDARD = c99
OPTIMIZATION = -O0
CFLAGS = -std=$(STANDARD) $(OPTIMIZATION) -MD -Wall -Werror -pedantic
RM = rm -rf
TARGET = hello

SRCDIR = src
OBJDIR = obj
BUILD = build

DIRSTRUCT = $(shell find $(SRCDIR) -type d)

SRCS = $(shell find $(SRCDIR) -name '*.c')
OBJS = $(patsubst %.c, $(OBJDIR)/%.o, $(SRCS))

all: $(BUILD)/$(TARGET)

$(BUILD)/$(TARGET): $(OBJS)
	@mkdir -p $(@D)
	@$(CC) $(OBJS) $(LDFLAGS) -o $@
	@echo 'Done!'

$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

.PHONY: all clean

clean:
	@$(RM) $(OBJDIR) $(BUILD)
	@echo 'Cleaning...Done!'

