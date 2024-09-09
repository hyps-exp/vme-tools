CFLAGS += -Wall -I../lib/
LDLIBS += ../lib/vmeg_lib.o ../lib/rbcp_lib.o

APS = v895_read # v265_clear v265_gg v265_read

.c.o:
	$(CC) $(CFLAGS) -c $*.c -o $@

all : $(APS)

clean :
	rm -f *.o *~ $(APS)
