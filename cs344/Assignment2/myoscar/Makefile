CC =			gcc 
GXX_FLAGS =		-std=gnu11 -g -Wall

SOURCE =		
TARGET =		myoscar
OUTPUT_DIR =	.

all: myfile.o cmd_line_args.o archive.o toc.o myoscar.o

	$(CC) -o $(OUTPUT_DIR)/$(TARGET) $(GXX_FLAGS) myfile.o toc.o cmd_line_args.o archive.o myoscar.o
    
myfile.o: 
	$(CC) -c myfile.h myfile.c

cmd_line_args.o: 
	$(CC) -c cmd_line_args.h cmd_line_args.c

myoscar.o: 
	$(CC) -c myoscar.c

archive.o: 
	$(CC) -c archive.h archive.c

toc.o: 
	$(CC) -c toc.h toc.c

debug: myfile.o cmd_line_args.o archive.o toc.o myoscar.o

	$(CC) -o $(OUTPUT_DIR)/$(TARGET) $(GXX_FLAGS) -g myfile.o toc.o cmd_line_args.o archive.o myoscar.o

clean:
	rm -rf ./*.h.gch ./*.o $(TARGET)

testa12345:
	rm -f oscar12345.oscar

	./oscar -a oscar12345.oscar 1-s.txt 2-s.txt 3-s.txt 4-s.txt 5-s.txt
	./myoscar -a myoscar12345.oscar 1-s.txt 2-s.txt 3-s.txt 4-s.txt 5-s.txt

	diff oscar12345.oscar myoscar12345.oscar

testT12345:
	rm -f oscar12345.oscar

	oscar -T oscar12345.oscar 1-s.txt 2-s.txt 3-s.txt 4-s.txt 5-s.txt
	oscar -T oscar12345.oscar > oscar-ltoc.txt
	myoscar -T oscar12345.oscar > myoscar-ltoc.txt

	diff oscar-ltoc.txt myoscar-ltoc.txt

tests: testa12345 testT12345
