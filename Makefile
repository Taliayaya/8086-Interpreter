CC = gcc
CFLAGS = -Wall -Wextra -g -std=c11 -I$(INCDIR) $(INCLUDE_FLAGS)
LDFLAGS =
LDLIBS =

ODIR 	= build
TESTDIR = tests
SRCDIR 	= src
INCDIR 	= include
LIBDIR 	= lib
BINDIR 	= bin

SRCS  	:= $(wildcard $(SRCDIR)/**/*.c $(SRCDIR)/*.c)
SRCDIRS := $(sort $(dir $(SRCS)))
OBJS 	:= $(patsubst $(SRCDIR)/%.c,$(ODIR)/%.o,$(SRCS))
LIBSINC	:= $(wildcard $(LIBDIR)/**/include/*.h)

LIB_DIRS := $(shell find $(LIBDIR) -type d -name 'include')

# Generate -I flags
INCLUDE_FLAGS := $(foreach dir,$(LIB_DIRS),-I $(dir))



# Test source files
TESTSRCS := $(wildcard $(TESTDIR)/*.c)
TESTOBJS := $(patsubst $(TESTDIR)/%.c, $(ODIR)/%.o, $(TESTSRCS))

# Object directory creation
$(shell mkdir -p $(ODIR) $(SRCDIRS:$(SRCDIR)%=$(ODIR)%))

# Rules
$(ODIR)/%.o: $(SRCDIR)/%.c
	$(CC) -c -o $@ $< $(CFLAGS)

# Targets
all: z124043_mmvm #$(TESTDIR)/utest

debug: 
	echo $(INCLUDE_FLAGS)

z124043_mmvm: $(OBJS)
	$(CC) -o $(BINDIR)/$@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o
	rm -rf $(ODIR)/*
	rm -f z124043_mmvm
	#rm -f $(TESTDIR)/utest

