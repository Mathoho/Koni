# COMPUTER SCIENCE 244
# Makefile for Tutorial 6
# (c) 2022 University of Stellenbosch

# compiler flags
DEBUGGER   = -ggdb
OPTIMISE   = -O0
WARNINGS   = -Wall -Wextra -pedantic -ansi
ARCH       = -m32
AFLAGS     = -g -f elf32
CFLAGS     = $(DEBUGGER) $(OPTIMISE) $(WARNINGS) $(ARCH)
DFLAGS     = # -DDEBUG

# commands
ASM        = nasm
CC         = clang
RM         = rm -rf
MKDIR      = mkdir -p
COMPILE    = $(CC) $(CFLAGS) $(DFLAGS)
ASSEMBLE   = $(ASM) $(AFLAGS)
INSTALL    = install

# directories
BINDIR     = ../bin
LOCALBIN   = ~/.local/bin

# targets
TARGETS    = hcompress hdecompress
LIBRARIES  = heap huffman
OBJECTS    = $(foreach OBJ, $(LIBRARIES), $(addsuffix .o, $(OBJ)))

### RULES ######################################################################

.PHONY: all clean install types uninstall

all: $(TARGETS)

$(TARGETS): %: %.c $(LIBRARIES) | $(BINDIR)
	$(COMPILE) -o $(BINDIR)/$@ $< $(OBJECTS)

$(LIBRARIES): %: %.asm %.h
	$(ASSEMBLE) $<

$(BINDIR):
	$(MKDIR) $(BINDIR)

clean:
	$(RM) $(foreach EXE, $(TARGETS), $(wildcard $(BINDIR)/$(EXE)))
	$(RM) $(foreach OBJ, $(OBJECTS), $(OBJ))

install:
	$(MKDIR) $(LOCALBIN)
	$(INSTALL) $(foreach EXE, $(TARGETS), $(wildcard $(BINDIR)/$(EXE))) \
		$(LOCALBIN)

uninstall:
	$(RM) $(foreach EXE, $(TARGETS), $(wildcard $(LOCALBIN)/$(EXE)))

# Note: Make a highlight file for user-defined types.  This requires Exuberant
# ctags and AWK.  To use this in Vim, add the following four lines to your
# .vimrc:
# au BufRead,BufNewFile *.[ch] let fname = expand('<afile>:p:h') . '/types.vim'
# au BufRead,BufNewFile *.[ch] if filereadable(fname)
# au BufRead,BufNewFile *.[ch]   exe 'so ' . fname
# au BufRead,BufNewFile *.[ch] endif
types: types.vim

types.vim: *.[ch]
	ctags --c-kinds=t -o- *.[ch] |\
		awk 'BEGIN{printf("syntax keyword cType\t")}\
		{printf("%s ", $$1)}\
		END{if(!NR){print "XXX_placeholder"}else{print ""}}' > $@
	ctags --c-kinds=gsu -o- *.[ch] |\
		awk 'BEGIN{printf("syntax keyword cStorageClass\t")}\
		{printf("%s ", $$1)}\
		END{if(!NR){print "XXX_placeholder"}else{print ""}}' >> $@
	ctags --c-kinds=e -o- *.[ch] |\
		awk 'BEGIN{printf("syntax keyword cConstant\t")}\
		{printf("%s ", $$1)}\
		END{if(!NR){print "XXX_placeholder"}else{print ""}}' >> $@
	ctags --c-kinds=d -o- *.[ch] |\
		awk 'BEGIN{printf("syntax keyword cDefine\t")}\
		{printf("%s ", $$1)}\
		END{if(!NR){print "XXX_placeholder"}else{print ""}}' >> $@
	echo -n "syntax keyword cDefine\tO_CREAT O_RDONLY O_RDWR O_TRUNC " >> $@
	echo "S_IRUSR S_IWUSR S_IRGRP S_IROTH" >> $@
