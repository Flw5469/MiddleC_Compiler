# Makefile with customizable filename
# Just specify NAME=yourfile when you run make

# Default name if none specified
NAME ?= output

# Define our tools and paths
NASM = nasm
GOLINK = GoLink\golink
ASM_FLAGS = -f win32
LINK_FLAGS = /entry start
BUILD_PATH = build/output
DEPENDENCIES_PATH = dependencies/

# Default target
all: $(BUILD_PATH)/$(NAME).exe

# Create build directory if it doesn't exist
$(BUILD_PATH):
	mkdir $(BUILD_PATH)

# Rule to build the executable
$(BUILD_PATH)/$(NAME).exe: $(BUILD_PATH)/$(NAME).obj
	$(DEPENDENCIES_PATH)$(GOLINK) $(LINK_FLAGS) $(BUILD_PATH)/$(NAME).obj user32.dll kernel32.dll /fo $(BUILD_PATH)/$(NAME).exe
	@echo Executable $(BUILD_PATH)/$(NAME).exe is ready for action!

# Rule to compile the assembly file
$(BUILD_PATH)/$(NAME).obj: $(BUILD_PATH)/$(NAME).asm | $(BUILD_PATH)
	$(NASM) $(ASM_FLAGS) $(BUILD_PATH)/$(NAME).asm -o $(BUILD_PATH)/$(NAME).obj

clean:
	rm -f "$(BUILD_PATH)/$(NAME).obj" "$(BUILD_PATH)/$(NAME).exe" "$(BUILD_PATH)/$(NAME).asm"
	@echo "Cleaned build files"

clean-all:
	rm -rf "$(BUILD_PATH)"
	@echo "Cleaned entire build directory"
# Phony targets
.PHONY: all clean clean-all