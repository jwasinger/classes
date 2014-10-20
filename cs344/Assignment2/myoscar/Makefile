CC =			gcc 
GXX_FLAGS =		-std=gnu11 -g -Wall

SOURCE =		
TARGET =		myoscar
OUTPUT_DIR =	./bin

myfile.o: 
	$(CC) -c myfile.h myfile.c

cmd_line_args.o: 
	$(CC) -c cmd_line_args.h cmd_line_args.c

myoscar.o: 
	$(CC) -c myoscar.c

debug: myfile.o cmd_line_args.o myoscar.o

	$(CC) -o $(OUTPUT_DIR)/$(TARGET) $(GXX_FLAGS) myfile.o cmd_line_args.o myoscar.o

clean:
	rm -rf ./*.h.gch ./*.o
