WKDIR=..
CC=gcc
COMPILE.c=$(CC) $(CFLAGS) $(CPPFLAGS) -c
LINK.c=$(CC) $(CFLAGS) $(CPPFLAGS) $(LDDIR) $(LDFLAGS)
LDDIR=-L../lib
LDLIBS=../lib/libapue.a $(EXTRALIBS)
CFLAGS=-DLINUX -ansi -I$(WKDIR)/include -Wall -D_GNU_SOURCE $(EXTRA)

# Our library that almost every program needs.
LIB=../libapue.a

# Common temp files to delete from each directory.
TEMPFILES=core core.* *.o temp.* *.out typescript*


EXTRA=
EXTRALIBS=-pthread

PROGS =	threadid exitstatus exitsleep cleanup badexit exp_sleep exp_exitstatus2 exp_exitstatus3 exp_exitstatus4 exp_exitstatus5 exp_exitstatus6

all:	condvar.o mutex1.o mutex2.o mutex3.o rwlock.o ${PROGS}

condvar.o:	condvar.c

mutex1.o:	mutex1.c

mutex2.o:	mutex2.c

mutex3.o:	mutex3.c

rwlock.o:	rwlock.c

clean:
	rm -f ${PROGS} ${TEMPFILES} *.o
