ifeq ($(KERNELRELEASE),)
#KERNEL_DIR ?= /lib/modules/$(shell uname -r)/build
KERNEL_DIR ?= /home/linux/drivers/linux-3.14/
PWD ?= $(shell pwd)

modules:
	make -C $(KERNEL_DIR) M=$(PWD) modules 

clean:
	make -C $(KERNEL_DIR) M=$(PWD) clean

else
obj-m += fs4412-adc.o
endif

