#KERNELDIR := /lib/modules/$(shell uname -r)/build/
KERNELDIR := /home/linux/work/kernel/linux-3.14-fs4412/

obj-m := ex29_platform_dts_workqueue.o
all:
	$(MAKE) -C $(KERNELDIR) M=$(shell pwd)

clean:
	rm -rf *.ko *.o Module* module* *.mod* .*.cmd .*versions
