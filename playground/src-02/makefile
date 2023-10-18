ASM = as
ASM_FLAGS = -g
LINKER = ld
RM = rm -rf
TARGET = test

all: $(TARGET)

$(TARGET): test.o
	@$(LD) $^ -o $@
	@echo 'Done!'

%.o: %.S
	@$(ASM) $(ASM_FLAGS) $^ -o $@

clean:
	@$(RM) *.o
	@$(RM) $(TARGET)
	@echo 'Cleaning ... Done!'

