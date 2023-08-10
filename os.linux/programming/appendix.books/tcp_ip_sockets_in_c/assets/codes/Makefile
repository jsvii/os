CC = gcc
IMPL_CFLAGS = -std=c99 -D_XOPEN_SOURCE=600 \
		-g \
		-pedantic \
		-Wall \
		-W \
		-Wpointer-arith \
		-Wmissing-prototypes \
		-Wno-sign-compare \
		-Wno-unused-parameter \
		-Wno-format-y2k \
		-Wno-long-long
RM = rm -f


EXE = TCPEchoServer4 TCPEchoClient4

all : ${EXE}

TCPEchoServer4: TCPEchoServer4.o  TCPServerUtility.o AddressUtility.o DieWithMessage.o

TCPEchoClient4: TCPEchoClient4.o DieWithMessage.o

%.o : %.c
	@echo
	$(CC) -c $(IMPL_CFLAGS) $< -o $@

clean :
	${RM} ${EXE} *.o

showall :
	@ echo ${EXE}

${EXE} : ${UTILITY_SOURCE}		# True as a rough approximation
