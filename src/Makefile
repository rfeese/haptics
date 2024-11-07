.PHONY: install clean

libhaptics.a: haptics.o
	ar cr $@ haptics.o

install: libhaptics.a haptics.h
	cp libhaptics.a $(DESTDIR)/lib/
	cp haptics.h $(DESTDIR)/include/

clean:
	- rm *.a
	- rm *.o
