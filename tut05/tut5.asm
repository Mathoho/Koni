; asmsyntax=nasm
;*****************************************************************************
;* CS244: Assembler implementation of external functions defined in          *
;*        'tut5.h'.                                                          *
;*****************************************************************************

extern free

global binary_sort
global delnode
global bin_to_string

;*****************************************************************************
;* void binary_sort(Item *list, int n);                                      *
;*****************************************************************************
binary_sort:
  ret

;*****************************************************************************
;* void rmmax(Node **root, Node **max);                                      *
;*****************************************************************************
rmmax:
  ret

;*****************************************************************************
;* void delnode(Node **root, const char *name);                              *
;*****************************************************************************
delnode:
  ret

;*****************************************************************************
;* void bin_to_string(int n, char *s);                                       *
;*****************************************************************************
bin_to_string:
  ret
