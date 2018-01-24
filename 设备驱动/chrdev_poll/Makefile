KERNELDIR := /lib/modules/$(shell uname -r)/build/

obj-m := ex21_chrdev_poll.o

all:
	$(MAKE) -C $(KERNELDIR) M=$(shell pwd)

clean:
	rm -rf *.ko *.o Module* module* *.mod* .*.cmd .*versions
