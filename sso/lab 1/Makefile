COMPILE = echo Kompilacja $@; \
		gcc -c -o $@ $<

LINKING = gcc -o $@ $^

all: run
.PHONY: run clean

run: main

clean:
	rm -fr *.o main

main: main.o header.o
	$(LINKING)

main.o: main.c header.h
	$(COMPILE)

header.o: header.c header.h
	$(COMPILE)