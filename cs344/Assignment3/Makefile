CC = gcc
GCC_FLAGS = -Wall -Wshadow -Wunreachable-code -Wmissing-declarations -Wold-style-definition -Wmissing-prototypes -Wdeclaration-after-statement
OUTPUT_DIR = ./bin
TARGET = a2

word_freq:

fifo_server:

fifo_client:

myword_freq:
	$(CC) -c -o myword_freq.o -g myword_freq.c

util:
	cd util
	make DynArr 

all: util myword_freq  

	mkdir -p $(OUTPUT_DIR)
	$(CC) -o $(OUTPUT_DIR)/$(TARGET) $(GCC_FLAGS) myword_freq.o util/DynArr.o 

clean:
	rm -rf $(OUTPUT_DIR)
