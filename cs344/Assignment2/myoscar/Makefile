CC =			gcc 
GXX_FLAGS =		-std=gnu11 -g

SOURCE =		
TARGET =		myoscar
OUTPUT_DIR =	.

all: myfile.o cmd_line_args.o archive.o toc.o myoscar.o

	$(CC) -o $(OUTPUT_DIR)/$(TARGET) $(GXX_FLAGS) myfile.o toc.o cmd_line_args.o archive.o myoscar.o

debug: 
	$(CC) -o $(OUTPUT_DIR)/$(TARGET) $(GXX_FLAGS) ./*.h ./*.c

clean:
	rm -rf ./*.h.gch ./*.o $(TARGET)

testa12345:
	rm -f oscar12345.oscar
	./oscar -a oscar12345.oscar 1-s.txt 2-s.txt 3-s.txt 4-s.txt 5-s.txt
	./myoscar -a myoscar12345.oscar 1-s.txt 2-s.txt 3-s.txt 4-s.txt 5-s.txt
	diff oscar12345.oscar myoscar12345.oscar

testT12345:
	rm -f oscar12345.oscar
	./oscar -a oscar12345.oscar 1-s.txt 2-s.txt 3-s.txt 4-s.txt 5-s.txt
	./oscar -T oscar12345.oscar > oscar-ltoc.txt
	./myoscar -T oscar12345.oscar > myoscar-ltoc.txt
	diff oscar-ltoc.txt myoscar-ltoc.txt

tests: testa12345 testT12345
