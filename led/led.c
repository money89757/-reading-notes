#include<linux/init.h>
#include<linux/module.h>
#include<linux/cdev.h>
#include<linux/fs.h>
#include<linux/device.h>
#include<asm/io.h>
#include"led_ioctl.h"

#define LED_MAJOR 250
#define LED_MINOR 0

#define GPX2CON 0x11000c40
#define GPX1CON 0x11000c20
#define GPF3CON 0x114001e0


volatile unsigned int *gpx2con;
volatile unsigned int *gpx2dat;

volatile unsigned int *gpx1con;
volatile unsigned int *gpx1dat;

volatile unsigned int *gpf3con;
volatile unsigned int *gpf3dat;

MODULE_LICENSE("GPL");

dev_t devno;
struct cdev led_cdev;

struct class *led_cls;
struct device *led_devs;

static int led_open(struct inode *inode,struct file *filp)
{
	printk("led_open success\n");
	return 0;
}

static int led_close(struct inode *inode,struct file *filp)
{
	printk("led_close success\n");
	return 0;
}

long led_ioctl(struct file *filp,unsigned int cmd,unsigned long arg)
{
	switch(cmd)
	{
	case LED2_ON:
		*gpx2dat = (*gpx2dat & ~(1 << 7)) | 1 << 7;
		*gpx1dat = *gpx1dat & ~(1 << 0);
		*gpf3dat = *gpf3dat & ~(3 << 4);
		break;
	case LED2_OFF:
		*gpx2dat = *gpx2dat & ~(1 << 7);
		*gpx1dat = *gpx1dat & ~(1 << 0);
		*gpf3dat = *gpf3dat & ~(3 << 4);
		break;
	case LED1_ON:
		*gpx2dat = *gpx2dat & ~(1 << 7);
		*gpx1dat = (*gpx1dat & ~(1 << 0)) | 1 << 0;
		*gpf3dat = *gpf3dat & ~(3 << 4);
		break;
	case LED1_OFF:
		*gpx2dat = *gpx2dat & ~(1 << 7);
		*gpx1dat = *gpx1dat & ~(1 << 0);
		*gpf3dat = *gpf3dat & ~(3 << 4);
		break;
	case LED3_ON:
		*gpx2dat = *gpx2dat & ~(1 << 7);
		*gpx1dat = *gpx1dat & ~(1 << 0);
		*gpf3dat = (*gpf3dat & ~(1 << 4)) | 1 << 4;
		*gpf3dat = *gpf3dat & ~(1 << 5);
		break;
	case LED3_OFF:
		*gpx2dat = *gpx2dat & ~(1 << 7);
		*gpx1dat = *gpx1dat & ~(1 << 0);
		*gpf3dat = *gpf3dat & ~(3 << 4);
		break;

		
	}
	return 0;
}

struct file_operations led_ops = {
	.open = led_open,
	.release = led_close,
	.unlocked_ioctl = led_ioctl,
};

static int led_init(void)
{
	int ret;
	devno = MKDEV(LED_MAJOR,LED_MINOR);

	ret = register_chrdev_region(devno,1,"led");
	if(ret != 0)
	{
		printk("register_chrdev_region fail\n");
		ret = alloc_chrdev_region(&devno,0,1,"led");
		if(ret != 0)
		{
			printk("alloc_chrdev_region fail\n");
			return -ENOMEM;
		}
	}

	cdev_init(&led_cdev,&led_ops);
	cdev_add(&led_cdev,devno,1);


	gpx2con = ioremap(GPX2CON,4);
	gpx2dat = gpx2con + 1;

	gpx1con = ioremap(GPX1CON,4);
	gpx1dat = gpx1con + 1;

	gpf3con = ioremap(GPF3CON,4);
	gpf3dat = gpf3con + 1;
	//设置引脚为输出模式
	*gpx2con = (*gpx2con & ~(0xf << 28)) | 1 << 28; 
	*gpx1con = (*gpx1con & ~(0xf << 0)) | 1 << 0;
	*gpf3con = (*gpf3con & ~(0xff << 16)) | 0x11 << 16;

	led_cls = class_create(THIS_MODULE,"led");
	if(IS_ERR(led_cls))
	{
		printk("class_create fail\n");
		return -EFAULT;
	}

	led_devs = device_create(led_cls,NULL,devno,NULL,"led");
	if(IS_ERR(led_devs))
	{
		printk("device_create fail\n");
		return -EFAULT;
	}
	printk("led_init\n");
	return 0;
}

module_init(led_init);

static void led_exit(void)
{
	device_destroy(led_cls,devno);
	class_destroy(led_cls);

	cdev_del(&led_cdev);

	unregister_chrdev_region(devno,1);
	printk("led_exit");
	return;
}
module_exit(led_exit);
