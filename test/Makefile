SHELL=/bin/sh
CC=$(CROSS)gcc
PKG_CONFIG=$(CROSS)pkg-config
CFLAGS=-g -Wall
UNITY=../../Unity/src/unity.c

.PHONY: all test clean

# default - run tests
all test:  test_haptics
	-./test_haptics

test_internal:  test_haptics_internal
	-./test_haptics_internal

# build tests
test_haptics: $(UNITY) test_haptics.c ../src/haptics.h ../src/haptics.c
	$(CC) $(CFLAGS) $(UNITY) test_haptics.c ../src/haptics.c -o test_haptics

test_haptics_internal: $(UNITY) test_haptics_internal.c ../src/haptics.h ../src/haptics.c
	$(CC) $(CFLAGS) $(UNITY) test_haptics_internal.c -o test_haptics_internal

# delete compiled binaries
clean test_clean:
	- rm test_haptics
	- rm test_haptics_internal
