all: skiplist.so

skiplist.so: skiplist.h skiplist.c lua-skiplist.c
	gcc -g3 -O0 -Wall -fPIC --shared $^ -o $@

test:
	lua test.lua

clean:
	-rm skiplist.so
