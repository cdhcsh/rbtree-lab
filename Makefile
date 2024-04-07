.PHONY: clean

CFLAGS=-Wall -g -DSENTINEL

execute:
	./driver

build : driver

driver: driver.o rbtree.o

clean:
	rm -f driver *.o
