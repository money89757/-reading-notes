ifeq ($(KERNELRELEASE),)
KERNEL_DIR ?= /lib/modules/$(shell uname -r)/build

PWD ?= $(shell pwd)

modules:
	make -C $(KERNEL_DIR) M=$(PWD) modules 

clean:
	make -C $(KERNEL_DIR) M=$(PWD) clean

else
obj-m += myplatform_device.o myplatform_driver.o
endif

