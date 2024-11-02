#sample Makefile.

COMPILER ?= $(GCC_PATH)gcc
FLAGS ?= -O3 -Wall $(GCC_SUPPFLAGS) #-DDEBUG

LDFLAGS ?= -g
LDLIBS =

EXECUTABLE = add_col_vals 

SRCS=$(wildcard src/*.c)
OBJS=$(SRCS:src/%.c=obj/%.o)

all: $(OBJS)
	$(COMPILER) $(LDFLAGS) -o $(EXECUTABLE) $(OBJS) $(LDLIBS) 

obj/%.o: src/%.c | obj
	$(COMPILER) $(FLAGS) -o $@ -c $<

obj:
	mkdir -p $@

clean:
	rm -f obj/*
	rm $(EXECUTABLE) 
	rm *.log

cleandata:
	rm cachegrind.out.*

