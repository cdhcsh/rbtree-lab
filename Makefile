.PHONY: clean

CFLAGS=-Wall -g -DSENTINEL

all: driver

execute:
	./driver

build : driver

driver: driver.o rbtree.o

clean:
	rm -f driver *.o
