CC = gcc
GCC_FLAGS = -pthread -Wall -Wshadow -Wunreachable-code -Wmissing-declarations -Wold-style-definition -Wmissing-prototypes -Wdeclaration-after-statement -pthread
OUTPUT_DIR = ./bin

IPDB_TARGET = ipdb

ipdb:
	mkdir -p $(OUTPUT_DIR)
	$(CC) -c -o $(OUTPUT_DIR)/ipdb.o $(GCC_FLAGS) -g ipdb.c

all: ipdb 
	mkdir -p $(OUTPUT_DIR)
	$(CC) -o $(OUTPUT_DIR)/$(IPDB_TARGET) $(GCC_FLAGS) $(OUTPUT_DIR)/ipdb.o -lrt 

clean:
	rm -rf $(OUTPUT_DIR)
