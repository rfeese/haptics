CC=$(CROSS)gcc
PKG_CONFIG=$(CROSS)pkg-config
CFLAGS=-g -Wall
LIBS = `$(PKG_CONFIG) sdl2 --libs`

.PHONY: all clean install test test_clean

#binaries
all: example

example: example.c src/haptics.o
	$(CC) $(LDFLAGS) example.c src/haptics.o $(LIBS) -o $@

install:
	$(MAKE) --directory src $@

#delete compiled binaries
clean:
	$(MAKE) --directory src $@
	- rm example

#buid and run tests
test:
	$(MAKE) --directory test $@

test_clean:
	$(MAKE) --directory test $@
