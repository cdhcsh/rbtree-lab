.PHONY: clean

CFLAGS=-Wall -g -DSENTINEL

test: clean build
	./driver
	valgrind ./driver

build : driver

driver: driver.o rbtree.o

clean:
	rm -f driver *.o
