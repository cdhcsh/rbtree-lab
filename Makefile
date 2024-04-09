.PHONY: clean

CFLAGS=-Wall -g -DSENTINEL

test: clean update build
	./driver
	valgrind ./driver

update:
	git pull

build : driver

driver: driver.o rbtree.o

clean:
	rm -f driver *.o
