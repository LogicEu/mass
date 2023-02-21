# mass makefile

NAME = libmass

CC = gcc
STD = -std=c99
WFLAGS = -Wall -Wextra -pedantic
OPT = -O2
INC = -I. -Iinclude
LIB = utopia fract

SRCDIR = src
TMPDIR = tmp
BINDIR = bin
LIBDIR = lib

SCRIPT = build.sh

SRC = $(wildcard $(SRCDIR)/*.c)
OBJS = $(patsubst $(SRCDIR)/%.c,$(TMPDIR)/%.o,$(SRC))
LIBS = $(patsubst %,$(LIBDIR)/lib%.a,$(LIB))
DLIB = $(patsubst %, -L%, $(LIBDIR))
DLIB += $(patsubst %, -l%, $(LIB))

OS=$(shell uname -s)
ifeq ($(OS),Darwin)
	DLIB += -dynamiclib
	SUFFIX = .dylib
else
	DLIB += -lm -shared -fPIC
	SUFFIX = .so
endif

TARGET = $(BINDIR)/$(NAME)
LIBNAME = $(TARGET)$(SUFFIX)

CFLAGS = $(STD) $(WFLAGS) $(OPT) $(INC)

$(TARGET).a: $(BINDIR) $(OBJS)
	ar -cr $@ $(OBJS)

.PHONY: shared all clean install uninstall

shared: $(LIBNAME)

all: $(LIBNAME) $(TARGET).a

$(LIBNAME): $(BINDIR) $(OBJS) $(LIBS)
	$(CC) $(CFLAGS) $(DLIB) -o $@ $(OBJS)

$(LIBDIR)/lib%.a: %
	cd $^ && $(MAKE) && mv $(BINDIR)/* ../$(LIBDIR)

$(LIBS): | $(LIBDIR)

$(TMPDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJS): | $(TMPDIR)

$(TMPDIR):
	mkdir -p $@

$(BINDIR):
	mkdir -p $@

$(LIBDIR):
	mkdir -p $@

clean: $(SCRIPT)
	./$^ $@

install: $(SCRIPT)
	./$^ $@

uninstall: $(SCRIPT)
	./$^ $@
