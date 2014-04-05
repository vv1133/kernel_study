obj-m=test2.o
test2-y=main.o

all:
	make -C /lib/modules/`uname -r`/build EXTRA_CFLAGS=-I$(shell pwd)/include M=`pwd`
clean:
	rm -f *.o *.ko *.mod.c modules.order Module.symvers

