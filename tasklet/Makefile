ifneq ($(KERNELRELEASE),)
	obj-m=test.o
	test-y=main.o
else
	KERDIR ?= /lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd)
all:
	make -C $(KERDIR) M=$(PWD) modules
clean:
	make -C $(KERDIR) M=$(PWD) clean
#	rm -f *.o *.ko *.mod.c modules.order Module.symvers

endif
