CC = gcc
GCC_FLAGS = -Wall -Wshadow -Wunreachable-code -Wmissing-declarations -Wold-style-definition -Wmissing-prototypes -Wdeclaration-after-statement
OUTPUT_DIR = .
TARGET = dyn_arr_test
SRC_DIR = .
INC_DIR = .

DynArr:
	$(CC) -c -o DynArr.o -g DynArr.c

DynArrTest:
	$(CC) -c -o DynArrTest.o -g DynArrTest.c

all: DynArr DynArrTest 
	$(CC) -o $(OUTPUT_DIR)/$(TARGET) $(GCC_FLAGS) -g DynArr.o DynArrTest.o

clean:
	rm -rf ./*.o $(TARGET)
