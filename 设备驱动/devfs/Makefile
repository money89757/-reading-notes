KERNELDIR := /lib/modules/$(shell uname -r)/build/

obj-m := ex30_devfs.o

all:
	$(MAKE) -C $(KERNELDIR) M=$(shell pwd)

clean:
	rm -rf *.ko *.o Module* module* *.mod* .*.cmd .*versions
