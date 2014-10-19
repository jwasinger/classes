CC = 			gcc 
GXX_FLAGS = 	-std=gnu11 -g -Wall

TARGET = 		myoscar
OUTPUT_DIR = 	./bin

myfile.o:
	$(CC) -C myfile.h myfile.c

debug:
	$(CC) -o $(OUTPUT_DIR)/$(TARGET) $(GXX_FLAGS) ./*.h ./*.c
clean:
	rm -rf ./bin/*
