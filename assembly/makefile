# Makefile with customizable filename
# Just specify NAME=yourfile when you run make

# Default name if none specified
NAME ?= first

# Define our tools and paths
NASM = nasm
GOLINK = ..\GoLink\golink
ASM_FLAGS = -f win32
LINK_FLAGS = /entry start

# Default target
all: $(NAME).exe

# Rule to build the executable
$(NAME).exe: $(NAME).obj
	$(GOLINK) $(LINK_FLAGS) $(NAME).obj user32.dll kernel32.dll
	@echo Executable $(NAME).exe is ready for action!

# Rule to compile the assembly file
$(NAME).obj: $(NAME).asm
	$(NASM) $(ASM_FLAGS) $(NAME).asm -o $(NAME).obj

# Clean up compiled files
clean:
	del $(NAME).obj $(NAME).exe

# Phony targets
.PHONY: all clean