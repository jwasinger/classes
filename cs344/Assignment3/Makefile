CC = gcc
GCC_FLAGS = -Wall -Wshadow -Wunreachable-code -Wmissing-declarations -Wold-style-definition -Wmissing-prototypes -Wdeclaration-after-statement
OUTPUT_DIR = ./bin
TARGET = a3

word_freq:

fifo_server:

fifo_client:

myword_freq:
	$(CC) -c -o myword_freq.o $(GCC_FLAGS) -g myword_freq.c

dynarr:
	$(CC) -c -o DynArr.o $(GCC_FLAGS) -g DynArr.c

all: dynarr myword_freq  

	mkdir -p $(OUTPUT_DIR)
	$(CC) -o $(OUTPUT_DIR)/$(TARGET) $(GCC_FLAGS) myword_freq.o DynArr.o 

clean:
	rm -rf $(OUTPUT_DIR)
