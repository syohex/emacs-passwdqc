EMACS_ROOT ?= ../..
EMACS ?= emacs

CC      = gcc
LD      = gcc
CPPFLAGS = -I$(EMACS_ROOT)/src
CFLAGS = -std=gnu99 -ggdb3 -Wall -fPIC $(CPPFLAGS)
LDFLAGS =

.PHONY : test

all: passwdqc-core.so

passwdqc-core.so: passwdqc-core.o
	$(LD) -shared $(LDFLAGS) -o $@ $^ -lpasswdqc

passwdqc-core.o: passwdqc-core.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	-rm -f passwdqc-core.so passwdqc-core.o

test:
	$(EMACS) -Q -batch -L . $(LOADPATH) \
		-l test/test-passwdqc.el \
		-f ert-run-tests-batch-and-exit
