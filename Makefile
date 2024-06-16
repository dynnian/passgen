all: passgen

WARNINGS = -Wall
DEBUG = -ggdb -fno-omit-frame-pointer
OPTIMIZE = -O2

passgen: Makefile passgen.c
	$(CC) -o $@ $(WARNINGS) $(DEBUG) $(OPTIMIZE) passgen.c

clean:
	rm -f passgen

install:
	echo "Installing is not supported"

run:
	./passgen
