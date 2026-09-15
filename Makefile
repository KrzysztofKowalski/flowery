# flowery — SDL3 spirograph renderer
CC      ?= cc
CFLAGS  ?= -O2 -std=c11 -Wall -Wextra
CFLAGS  += $(shell pkg-config --cflags sdl3)
LDLIBS  += $(shell pkg-config --libs sdl3) -lm

SRC = src/main.c src/flowery.c
HDR = src/flowery.h

flowery: $(SRC) $(HDR)
	$(CC) $(CFLAGS) -o $@ $(SRC) $(LDLIBS)

check:
	./test.sh

clean:
	rm -f flowery
	rm -rf .build

.PHONY: check clean
