#include<linux/init.h>
#include<linux/module.h>
#include<linux/platform_device.h>
#include<linux/cdev.h>
#include<linux/fs.h>
#include<asm/uaccess.h>
#include<linux/interrupt.h>
#include<linux/sched.h>

#define KEY_MAJOR 500
#define KEY_MINOR 0

#define CLASS_NAME "key"

struct cdev key_cdev;
struct class *key_cls;
struct device *key_devs;
dev_t devno;

wait_queue_head_t keyq;
int flag = 0;

struct resource *res_key1;
struct resource *res_key2;

int key;

MODULE_LICENSE("GPL");

int key_open(struct inode *inode,struct file *filp)
{
	printk("key_open success\n");
	return 0;
}

ssize_t key_read(struct file *filp,char __user *ubuf,size_t size,loff_t *off)
{
	int ret;
	
	wait_event_interruptible(keyq,flag != 0);
	ret = copy_to_user(ubuf,&key,size);
	if(ret)
	{
		printk("copy_to_user fail\n");
		return -EFAULT;
	}
	flag = 0;
	return size;
}

int key_close(struct inode *inode,struct file *filp)
{
	printk("key_close success\n");
	return 0;
}

#if 1
irqreturn_t key_handler(int irqnum,void *dev)
{
	if(irqnum == res_key1->start)
		key = 1;

	if(irqnum == res_key2->start)
		key = 2;
	
	flag = 1;
	wake_up_interruptible(&keyq);
	
	return IRQ_HANDLED;
}
#endif
struct file_operations key_ops = {
	.open = key_open,
	.read = key_read,
	.release = key_close,
};

int key_probe(struct platform_device *key_dev)
{
	int ret;
	printk("match ok\n");
	
	devno = MKDEV(KEY_MAJOR,KEY_MINOR);
	ret = register_chrdev_region(devno,1,"key");
	
	cdev_init(&key_cdev,&key_ops);	
	cdev_add(&key_cdev,devno,1);	

	key_cls = class_create(THIS_MODULE,CLASS_NAME);
	key_devs = device_create(key_cls,NULL,devno,NULL,"key");
	
	init_waitqueue_head(&keyq);
	
	//struct resource结构体首地址，这个结构体内部存中断号
	res_key1 = platform_get_resource(key_dev,IORESOURCE_IRQ,0);	
	res_key2 = platform_get_resource(key_dev,IORESOURCE_IRQ,1);	
	
	ret = request_irq(res_key1->start,key_handler,IRQF_DISABLED | IRQF_TRIGGER_FALLING,"keyno1",NULL);
	ret = request_irq(res_key2->start,key_handler,IRQF_DISABLED | IRQF_TRIGGER_FALLING,"keyno2",NULL);

	printk("key_probe success\n");
	return 0;
}

int key_remove(struct platform_device *key_dev)
{
	device_destroy(key_cls,devno);
	class_destroy(key_cls);
	cdev_del(&key_cdev);
	unregister_chrdev_region(devno,1);
	printk("key_remove success\n");
	return 0;
}
struct of_device_id key_of_table[] = {
	{
		.compatible = "farsight,key",
	},
};

struct platform_driver key_drv = {
	.driver = {
		.name = "17022-key",
		.of_match_table = key_of_table,
	},

	.probe = key_probe,
	.remove = key_remove,
};

int mykey_init(void)
{
	platform_driver_register(&key_drv);
	printk("key_init\n");
	return 0;
}
module_init(mykey_init);

void mykey_exit(void)
{
	platform_driver_unregister(&key_drv);
	printk("key_exit\n");
	return;
}
module_exit(mykey_exit);

