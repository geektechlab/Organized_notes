CC=gcc
CFLAGS=-g
OUTFILES=test_application.exe gcl.a
EXTERNAL_LIBS=
OBJS=Library/gcl.o

TARGET:test_application.exe gcl.a

test_application.exe:test_application.o ${OBJS}
	${CC} ${CFLAGS} test_application.o ${OBJS} -o test_application.exe ${EXTERNAL_LIBS}
test_application.o:test_application.c
	${CC} ${CFLAGS} -c -I Library test_application.c -o test_application.o
Library/gcl.o:Library/gcl.c
	${CC} ${CFLAGS} -c -I Library Library/gcl.c -o Library/gcl.o
gcl.a:${OBJ}
	ar rs gcl.a ${OBJ}
clean:
	rm -f test_application.o
	rm -f ${OUTFILES}
	rm -f ${OBJS}
