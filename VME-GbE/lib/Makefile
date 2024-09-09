CFLAGS += -Wall

LIBS = vmeg_lib.o rbcp_lib.o

.c.o:
	$(CC) $(CFLAGS) -c $*.c -o $@

all: $(LIBS)

clean:
	rm -f *.[ado] *~
