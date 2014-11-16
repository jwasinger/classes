CC = gcc
GCC_FLAGS = -Wall -Wshadow -Wunreachable-code -Wmissing-declarations -Wold-style-definition -Wmissing-prototypes -Wdeclaration-after-statement
OUTPUT_DIR = ./bin

MYWORD_FREQ_TARGET = myword_freq
FIFO_CLIENT_TARGET = fifo_client
FIFO_SERVER_TARGET = fifo_server

word_freq:

fifo_client:
	mkdir -p $(OUTPUT_DIR)
	$(CC) -c -o $(OUTPUT_DIR)/fifo_client.o $(GCC_FLAGS) -g fifo_client.c

fifo_server:
	mkdir -p $(OUTPUT_DIR)
	$(CC) -c -o $(OUTPUT_DIR)/fifo_server.o $(GCC_FLAGS) -g fifo_server.c

myword_freq:
	mkdir -p $(OUTPUT_DIR)
	$(CC) -c -o $(OUTPUT_DIR)/myword_freq.o $(GCC_FLAGS) -g myword_freq.c

dynarr:
	mkdir -p $(OUTPUT_DIR)
	$(CC) -c -o DynArr.o $(GCC_FLAGS) -g DynArr.c

all: myword_freq fifo_client fifo_server 

	mkdir -p $(OUTPUT_DIR)
	
	$(CC) -o $(OUTPUT_DIR)/$(MYWORD_FREQ_TARGET) $(GCC_FLAGS) $(OUTPUT_DIR)/myword_freq.o 
	$(CC) -o $(OUTPUT_DIR)/$(FIFO_CLIENT_TARGET) $(GCC_FLAGS) $(OUTPUT_DIR)/fifo_client.o
	$(CC) -o $(OUTPUT_DIR)/$(FIFO_SERVER_TARGET) $(GCC_FLAGS) $(OUTPUT_DIR)/fifo_server.o 

clean:
	rm -rf $(OUTPUT_DIR)
