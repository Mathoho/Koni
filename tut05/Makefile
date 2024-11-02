# COMPUTER SCIENCE 244
# Makefile for Tutorial 5
# (c) 2022 University of Stellenbosch

# flags
DEBUG     = -ggdb
OPTIMISE  = -O0
TARGET    = -m32
WARNINGS  = -Wall -Werror -Wextra -ansi -pedantic
CFLAGS    = $(DEBUG) $(OPTIMISE) $(WARNINGS) $(TARGET)
DFLAGS    = # nothing yet....
ASMFLAGS  = -f elf32

# commands
ASM       = nasm
CC        = clang
RM        = rm -f
COMPILE   = $(CC) $(CFLAGS) $(DFLAGS)
ASSEMBLE  = $(ASM) $(ASMFLAGS)

# files
ASM_SRCS  = tut5
C_LIBS    = strtoi
TARGETS   = b2s bsort btree
N_PREFIX  = test_
TESTDIR   = tests

ASM_OBJS  = $(addsuffix .o, $(ASM_SRCS))
C_OBJS    = $(addsuffix .o, $(C_LIBS))
EXES      = $(addprefix $(N_PREFIX), $(TARGETS))

# rules ########################################################################

.PHONY: all clean test

all: $(TARGETS) $(EXES)

test: $(EXES)

$(TARGETS): %: $(N_PREFIX)%.c $(ASM_OBJS) $(C_OBJS)
	$(COMPILE) -o $(<:.c=) $< $(ASM_OBJS) $(C_OBJS)

$(ASM_OBJS): %.o: %.asm %.h
	$(ASSEMBLE) $<

$(C_OBJS): %.o: %.c %.h
	$(COMPILE) -c $<

$(EXES): $(N_PREFIX)%: %
	@echo
	@echo "\033[33m"Testing $< ..."\033[m"
	@if [ -d $(TESTDIR)/$</cmd ]; then \
		for TESTCASE in $$(ls $(TESTDIR)/$</cmd); do \
			timeout 1s ./$@ $$(cat $(TESTDIR)/$</cmd/$$TESTCASE); \
	 	done \
	 fi
	@TPATH=$(TESTDIR)/$</in; if [ -d $$TPATH ]; then \
		for TESTCASE in $$(ls $$TPATH); do \
			TEST=$$TPATH/$$TESTCASE; \
			timeout 1s ./$@ $$(wc -l <$$TEST) <$$TEST; \
	 	done \
	 fi

clean:
	$(RM) *.o *.map *.s *.bin
	$(RM) $(EXES)
