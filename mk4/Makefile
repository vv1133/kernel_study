obj-m=mk4.o
mk4-y=main.o

all:
	make -C /lib/modules/`uname -r`/build EXTRA_CFLAGS=-I$(shell pwd)/include M=`pwd`
clean:
	rm -f *.o *.ko *.mod.c modules.order Module.symvers

