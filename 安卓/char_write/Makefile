KERNELDIR ?= /lib/modules/$(shell uname -r)/build

modules:
	$(MAKE) -C $(KERNELDIR) M=$(shell   pwd)  modules 

clean:
	$(MAKE) -C $(KERNELDIR) M=$(shell   pwd)  clean

    
	
obj-m := hello.o
