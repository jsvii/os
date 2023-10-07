CC=gcc
COMPILE.c=$(CC) $(CFLAGS) $(CPPFLAGS) -c
LINK.c=$(CC) $(CFLAGS) $(CPPFLAGS) $(LDDIR) $(LDFLAGS)
LDDIR=-L../lib
CFLAGS=-DLINUX -ansi -I../include -Wall -D_GNU_SOURCE $(EXTRA)

# Common temp files to delete from each directory.
TEMPFILES=core core.* *.o temp.* *.out typescript*
#
# Makefile for misc library.
#

AR	= ar
LIBMISC	= libapue.a
OBJS   = bufargs.o cliconn.o clrfl.o \
			 daemonize.o error.o errorlog.o lockreg.o locktest.o \
			 openmax.o pathalloc.o popen.o prexit.o prmask.o \
			 ptyfork.o linux_ptyopen.o readn.o recvfd.o \
			 senderr.o sendfd.o servaccept.o servlisten.o \
			 setfl.o signal.o signalintr.o \
			 sleepus.o spipe.o \
			 tellwait.o ttymodes.o writen.o # pathconf.o sysconf.o strerror.o 

RANLIB     = ranlib

all:	${OBJS}
	${AR} rv ${LIBMISC} $?
	${RANLIB} ${LIBMISC}

ptyopen.c:	linux_ptyopen.c
		cp linux_ptyopen.c ptyopen.c

clean:
	rm -f *.o a.out core temp.* ptyopen.c $(LIBMISC)
