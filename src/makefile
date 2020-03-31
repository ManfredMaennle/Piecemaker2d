#
# makefile -- rules to build piecemaker2d
#
# Copyright (c) 1999, 2020 Manfred Maennle, Karlsruhe/Kandel, Germany
#
# Permission is hereby granted, free of charge, to any person obtaining
# a copy of this software and associated documentation files (the
# "Software"), to deal in the Software without restriction, including
# without limitation the rights to use, copy, modify, merge, publish,
# distribute, sublicense, and/or sell copies of the Software, and to
# permit persons to whom the Software is furnished to do so, subject to
# the following conditions:
# 
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
# LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
# OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
# WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#
#
# $Id:$
#


### destinations

#BINDEST = /usr/local/bin
BINDEST = ../bin

### possibly use other tools

CC	= gcc
CCPP	= g++
YACC	= bison -b y
LEX	= flex
STRIP	= strip
COPY	= cp
RM	= rm -f


### binary flags 32 / 64 bit
BITS	= 32
#BITS	= 64


# WIN32

### OS depending compiler directives 
ifeq ($(OS),Windows_NT)
  BINFLAGS = -m$(BITS) -DWIN32  # -DDOS
else ### for unix/linux environment
  BINFLAGS = -m$(BITS)
endif

### machine type defines destination directory to copy executables into
#MACH	= "sun4"
#MACH	= "i586"
MACH	= unknown
ifeq ($(OS),Windows_NT)
  MACH = win$(BITS)
else ### for unix/linux environment
  MACH = $(HOSTTYPE)
endif

### suffix for binary executable
SUFFIX = 
ifeq ($(OS),Windows_NT)
  SUFFIX = .exe
else ### Unix
  SUFFIX = 
endif

### RCS treats suffixes differently in Windows and Unix environments
ifeq ($(OS),Windows_NT)
  RCSSUFF = 
  RCSXOPT = -x,v -zUTC
  RCSPATH = 
else ### for unix/linux environment
  RCSSUFF = ,v
  RCSXOPT = -zUTC
  RCSPATH = RCS/
endif


######## if needed, change the compiler options to your system's needs

### optimize; final compilation; SKIP_SAME_SIZE optimization was introduced with v 2.7
#COPT	= -O3 -fomit-frame-pointer
COPT	= -O3 -fomit-frame-pointer -DSKIP_SAME_SIZE

### check with lint
#CFLAGS  = -pg -DNDEBUG
#LDFLAGS = -pg

### conditional compilation flags
### $(MAKE_CFLAGS) is taken from the shell environment 
### (it is to be set by the calling script before recompilation)
#CFLAGS	= -g -Wall -pedantic $(MAKE_CFLAGS) -DYYDEBUG $(BINFLAGS)
#CFLAGS	= -g -Wall -pedantic $(MAKE_CFLAGS) -DTRACE_LEX -DTRACE_YACC $(BINFLAGS)
#CFLAGS	= -g -Wall -pedantic $(MAKE_CFLAGS) -DTRACE_FRAG $(BINFLAGS)
#CFLAGS	= -Wall -pedantic $(MAKE_CFLAGS) $(BINFLAGS)
CFLAGS	= -DNDEBUG -Wall -pedantic $(MAKE_CFLAGS) $(BINFLAGS)

YFLAGS	= -d -v
LXFLAGS	= -t

#INCPATH = -I../../../../programs-usb/MinGW/msys/1.0/include
INCPATH = 


### conditional link falgs
LDFLAGS = -Wall $(BINFLAGS)

LIBPATH	= 

LIBS 	= # -lm -lfl


################################################################################
###
### you usually don't need to change anything below
###
################################################################################

PROG	= piecemaker2d

OBJS	= main.o frag.o cut.o bbsort.o in_parse.o in_lex.o cf_parse.o cf_lex.o \
	  intvect.o rctarray.o rect.o etc.o strres.o

all:  $(PROG)$(SUFFIX)  # doc test

install: all
	$(COPY) $(PROG)$(SUFFIX) $(BINDEST)/$(MACH) 
	$(STRIP) $(BINDEST)/$(MACH)/$(PROG)$(SUFFIX) 


### check in, check out

ci: clean binclean texclean
	ci $(RCSXOPT) bbsort.c
	ci $(RCSXOPT) bbsort.h
	ci $(RCSXOPT) cf_lex.h
	ci $(RCSXOPT) cf_lex.l
	ci $(RCSXOPT) cf_parse.y
	ci $(RCSXOPT) cf_prs_h.h
	ci $(RCSXOPT) cut.c
	ci $(RCSXOPT) cut.h
	ci $(RCSXOPT) etc.c
	ci $(RCSXOPT) etc.h
	ci $(RCSXOPT) frag.c
	ci $(RCSXOPT) frag.h
	ci $(RCSXOPT) in_lex.h
	ci $(RCSXOPT) in_lex.l
	ci $(RCSXOPT) in_parse.y
	ci $(RCSXOPT) in_prs_h.h
	ci $(RCSXOPT) intvect.c
	ci $(RCSXOPT) intvect.h
	ci $(RCSXOPT) main.c
	ci $(RCSXOPT) main.h
	ci $(RCSXOPT) rctarray.c
	ci $(RCSXOPT) rctarray.h
	ci $(RCSXOPT) rect.c
	ci $(RCSXOPT) rect.h
	ci $(RCSXOPT) revision.h
	ci $(RCSXOPT) strres.c
	ci $(RCSXOPT) strres.h
	# ci $(RCSXOPT) makefile
	# $(RM) *.y *.l *.h *.c *.hh *.cc makefile

co:
	# co RCS/*,v
	co $(RCSXOPT) $(RCSPATH)bbsort.c$(RCSSUFF)
	co $(RCSXOPT) $(RCSPATH)bbsort.h$(RCSSUFF)
	co $(RCSXOPT) $(RCSPATH)cf_lex.h$(RCSSUFF)
	co $(RCSXOPT) $(RCSPATH)cf_lex.l$(RCSSUFF)
	co $(RCSXOPT) $(RCSPATH)cf_parse.y$(RCSSUFF)
	co $(RCSXOPT) $(RCSPATH)cf_prs_h.h$(RCSSUFF)
	co $(RCSXOPT) $(RCSPATH)cut.c$(RCSSUFF)
	co $(RCSXOPT) $(RCSPATH)cut.h$(RCSSUFF)
	co $(RCSXOPT) $(RCSPATH)etc.c$(RCSSUFF)
	co $(RCSXOPT) $(RCSPATH)etc.h$(RCSSUFF)
	co $(RCSXOPT) $(RCSPATH)frag.c$(RCSSUFF)
	co $(RCSXOPT) $(RCSPATH)frag.h$(RCSSUFF)
	co $(RCSXOPT) $(RCSPATH)in_lex.h$(RCSSUFF)
	co $(RCSXOPT) $(RCSPATH)in_lex.l$(RCSSUFF)
	co $(RCSXOPT) $(RCSPATH)in_parse.y$(RCSSUFF)
	co $(RCSXOPT) $(RCSPATH)in_prs_h.h$(RCSSUFF)
	co $(RCSXOPT) $(RCSPATH)intvect.c$(RCSSUFF)
	co $(RCSXOPT) $(RCSPATH)intvect.h$(RCSSUFF)
	co $(RCSXOPT) $(RCSPATH)main.c$(RCSSUFF)
	co $(RCSXOPT) $(RCSPATH)main.h$(RCSSUFF)
	co $(RCSXOPT) $(RCSPATH)rctarray.c$(RCSSUFF)
	co $(RCSXOPT) $(RCSPATH)rctarray.h$(RCSSUFF)
	co $(RCSXOPT) $(RCSPATH)rect.c$(RCSSUFF)
	co $(RCSXOPT) $(RCSPATH)rect.h$(RCSSUFF)
	co $(RCSXOPT) $(RCSPATH)strres.c$(RCSSUFF)
	co $(RCSXOPT) $(RCSPATH)strres.h$(RCSSUFF)
	co $(RCSXOPT) $(RCSPATH)revision.h$(RCSSUFF)


co-l: cleanall
	co -l $(RCSXOPT) $(RCSPATH)bbsort.c$(RCSSUFF)
	co -l $(RCSXOPT) $(RCSPATH)bbsort.h$(RCSSUFF)
	co -l $(RCSXOPT) $(RCSPATH)cf_lex.h$(RCSSUFF)
	co -l $(RCSXOPT) $(RCSPATH)cf_lex.l$(RCSSUFF)
	co -l $(RCSXOPT) $(RCSPATH)cf_parse.y$(RCSSUFF)
	co -l $(RCSXOPT) $(RCSPATH)cf_prs_h.h$(RCSSUFF)
	co -l $(RCSXOPT) $(RCSPATH)cut.c$(RCSSUFF)
	co -l $(RCSXOPT) $(RCSPATH)cut.h$(RCSSUFF)
	co -l $(RCSXOPT) $(RCSPATH)etc.c$(RCSSUFF)
	co -l $(RCSXOPT) $(RCSPATH)etc.h$(RCSSUFF)
	co -l $(RCSXOPT) $(RCSPATH)frag.c$(RCSSUFF)
	co -l $(RCSXOPT) $(RCSPATH)frag.h$(RCSSUFF)
	co -l $(RCSXOPT) $(RCSPATH)in_lex.h$(RCSSUFF)
	co -l $(RCSXOPT) $(RCSPATH)in_lex.l$(RCSSUFF)
	co -l $(RCSXOPT) $(RCSPATH)in_parse.y$(RCSSUFF)
	co -l $(RCSXOPT) $(RCSPATH)in_prs_h.h$(RCSSUFF)
	co -l $(RCSXOPT) $(RCSPATH)intvect.c$(RCSSUFF)
	co -l $(RCSXOPT) $(RCSPATH)intvect.h$(RCSSUFF)
	co -l $(RCSXOPT) $(RCSPATH)main.c$(RCSSUFF)
	co -l $(RCSXOPT) $(RCSPATH)main.h$(RCSSUFF)
	co -l $(RCSXOPT) $(RCSPATH)rctarray.c$(RCSSUFF)
	co -l $(RCSXOPT) $(RCSPATH)rctarray.h$(RCSSUFF)
	co -l $(RCSXOPT) $(RCSPATH)rect.c$(RCSSUFF)
	co -l $(RCSXOPT) $(RCSPATH)rect.h$(RCSSUFF)
	co -l $(RCSXOPT) $(RCSPATH)strres.c$(RCSSUFF)
	co -l $(RCSXOPT) $(RCSPATH)strres.h$(RCSSUFF)
	co -l $(RCSXOPT) $(RCSPATH)revision.h$(RCSSUFF)



################################################################################

##.SUFFIXES: .o .c .h .l .y .dtx .tex .dvi .ins .sty


### link objects

$(PROG)$(SUFFIX): $(OBJS) 
	$(CCPP) -o $@ $(COPT) $(LDFLAGS) $(LIBPATH) $(OBJS) $(LIBS)


### compilation rules

%.o: %.cc
	$(CCPP) $(COPT) $(CFLAGS) $(INCPATH) -c $<

%.o: %.c
	$(CC) $(COPT) $(CFLAGS) $(INCPATH) -c $<

%.c: %.l
	$(RM) $@
	$(LEX) $(LXFLAGS) $< > $@
#	$(COPY) $@ $@_copy.c

#%.c %.h: %.y
#	$(YACC) $(YFLAGS) $< -o $@

in_parse.c: in_parse.y
	$(YACC) $(YFLAGS) -p in_ $< -o $@

cf_parse.c: cf_parse.y
	$(YACC) $(YFLAGS) -p cf_ $< -o $@

#%parse.c: %parse.y
#	$(YACC) $(YFLAGS) -p `basename $@ parse.c` $< -o $@


# make documentation

#doc: dict.dvi
#dict.dvi:
#	$(LATEX) dict.tex
#	$(LATEX) dict.tex


# make tests

test: all
	./$(PROG)$(SUFFIX)


# clean up

cleanall: clean binclean texclean

clean: 
	$(RM) *~ *% *.bak *.out *.o core 
	$(RM) in_parse.c in_parse.cc in_parse.h in_parse.hh in_parse.out* in_lex.c in_lex.cc in_lex.c_copy.c
	$(RM) cf_parse.c cf_parse.cc cf_parse.h cf_parse.hh cf_parse.out* cf_lex.c cf_lex.cc cf_lex.c_copy.c

binclean: force
	$(RM) $(PROG) $(PROG)$(SUFFIX)

texclean: force
	$(RM) *.dvi *.aux  *.log *.toc

force:

################################################################################
### file dependencies

etc.o:		makefile main.h etc.h etc.c
strres.o:	makefile strres.h strres.c
rect.o:		makefile etc.h rect.h rect.c
rctarray.o:	makefile etc.h rect.h rctarray.h rctarray.c
intvect.o:	makefile etc.h intvect.h intvect.c
bbsort.o:	makefile intvect.h rect.h rctarray.h revision.h bbsort.h bbsort.c 
cut.o:		makefile etc.h strres.h main.h cut.h cut.c
frag.o:		makefile etc.h rect.h rctarray.h intvect.h cut.h revision.h frag.h frag.c
in_parse.o:	makefile strres.h rect.h in_lex.h in_prs_h.h in_parse.y
in_lex.o:	makefile etc.h rect.h in_parse.h in_lex.h in_lex.l
cf_parse.o:	makefile strres.h cf_lex.h cf_prs_h.h cf_parse.y
cf_lex.o:	makefile etc.h cf_parse.h cf_lex.h cf_lex.l
main.o:		makefile strres.h etc.h rect.h rctarray.h intvect.h cut.h frag.h \
			bbsort.h in_lex.h in_prs_h.h cf_lex.h cf_prs_h.h \
			revision.h main.h main.c 

