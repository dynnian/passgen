PREFIX ?= /usr/local
WARNINGS = -Wall
DEBUG = -ggdb -fno-omit-frame-pointer
OPTIMIZE = -O2

all: passgen

passgen: Makefile passgen.c
	$(CC) -o $@ $(WARNINGS) $(DEBUG) $(OPTIMIZE) passgen.c

clean:
	rm -f passgen

install: all
	install -Dm755 passgen $(DESTDIR)$(PREFIX)/bin/passgen
	install -Dm644 passgen.1 $(DESTDIR)$(PREFIX)/share/man/man1/passgen.1

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/passgen
	rm -f $(DESTDIR)$(PREFIX)/share/man/man1/passgen.1

run:
	./passgen
