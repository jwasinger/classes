CC =			gcc 
GXX_FLAGS =		-std=gnu11 -g -Wall

SOURCE =		
TARGET =		myoscar
OUTPUT_DIR =	.

myfile.o: 
	$(CC) -g -c myfile.h myfile.c

cmd_line_args.o: 
	$(CC) -g -c cmd_line_args.h cmd_line_args.c

myoscar.o: 
	$(CC) -g -c myoscar.c

archive.o: 
	$(CC) -g -c archive.h archive.c

toc.o: 
	$(CC) -g -c toc.h toc.c

debug: myfile.o cmd_line_args.o archive.o toc.o myoscar.o

	$(CC) -o $(OUTPUT_DIR)/$(TARGET) $(GXX_FLAGS) myfile.o toc.o cmd_line_args.o archive.o myoscar.o

clean:
	rm -rf ./*.h.gch ./*.o $(TARGET)
