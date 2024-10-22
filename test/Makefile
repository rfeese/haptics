SHELL=/bin/sh
CC=$(CROSS)gcc
PKG_CONFIG=$(CROSS)pkg-config
CFLAGS=-g -Wall
UNITY=../../Unity/src/unity.c

.PHONY: all test clean

# default - run tests
all test:  test_haptics
	-./test_haptics


# build a test
test_haptics: $(UNITY) test_haptics.c ../src/haptics.h ../src/haptics.c
	$(CC) $(CFLAGS) $(UNITY) test_haptics.c ../src/haptics.c -o test_haptics

# delete compiled binaries
clean test_clean:
	- rm test_haptics
