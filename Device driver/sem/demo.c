#include<linux/fs.h>
#include<linux/module.h>
#include<linux/init.h>
#include<linux/cdev.h>
#include<linux/slab.h>
#include<linux/device.h>
#include<asm/uaccess.h>
#include<asm/atomic.h>
#include<linux/mutex.h>
#include<linux/delay.h>
#include<linux/semaphore.h>


#define DEMO_MAJOR 255
#define DEMO_MINOR 0

dev_t devno = MKDEV(DEMO_MAJOR,DEMO_MINOR);
struct cdev *demo_cdev;

struct class *demo_cls;
struct device *demo_devs;

char kbuf[1024];
int len;


struct semaphore sem1;
struct semaphore sem2;

MODULE_LICENSE("GPL");

static int demo_open(struct inode *inode,struct file *filp)
{

	len = 0;
	printk("demo_open\n");
	return 0;
}
static ssize_t demo_read(struct file *filp,char __user *ubuf,size_t size,loff_t *off)
{
	int n;
	int ret;
	char *tmp;
	
//	down(&sem2);
	down_interruptible(&sem2);
	if(*off == len)
		*off = 0;

	if(size > len - *off)
		n = len - *off;
	else 
		n = size;
	tmp = kbuf + *off; //每次读取时的起始地址
	ret = copy_to_user(ubuf,tmp,n);
	if(ret != 0)
	{
		printk("copy_to_user fail\n");
		return -EFAULT;
	}

	*off += n;
	up(&sem1);
	printk("demo_read success\n");
	return n;
}

static ssize_t demo_write(struct file *filp,const char __user *ubuf,size_t size,loff_t *off)
{
	int ret;
	int n;
	char *tmp;
	
//	down(&sem1);
	down_interruptible(&sem1);
	ssleep(5);
	if(*off == sizeof(kbuf))
		return -ENOMEM;
	if(size > sizeof(kbuf) - *off)
		n = sizeof(kbuf) - *off;
	else 
		n = size;
	tmp = kbuf + *off;
	
	ret = copy_from_user(tmp,ubuf,n);
	if(ret != 0)
	{
		printk("copy_from_user fail\n");
		return -EFAULT;
	}

	*off += n;
	len += n;//控制一共写入的字节数	
	printk("driver write:len = %d,kbuf:%s\n",len,kbuf);
	printk("demo_write success\n");
	up(&sem2);
	return n;//返回值为正确写入到驱动空间的字节个数

}
static int demo_close(struct inode *inode,struct file *filp)
{
	printk("demo_close\n");
	return 0;
}


struct file_operations demo_ops = {
	.open = demo_open,
	.read = demo_read,
	.write = demo_write,
	.release = demo_close,
};

static int demo_init(void)
{
	int ret;
	ret = register_chrdev_region(devno,1,"demo");
	if(ret != 0)
	{
		printk("register_chrdev_region fail\n");
		ret = alloc_chrdev_region(&devno,0,1,"demo");
		if(ret != 0)
		{
			printk("alloc_chrdev_region fail\n");
			return -ENOMEM;
		}
	}
	
	demo_cdev = kzalloc(sizeof(struct cdev),GFP_KERNEL);
	if(IS_ERR(demo_cdev)) //返回值为非0代表出错
	{
		ret = PTR_ERR(demo_cdev);
		goto err1;
	}

	cdev_init(demo_cdev,&demo_ops);
	ret = cdev_add(demo_cdev,devno,1);
	if(ret != 0)
	{
		printk("cdev_add fail\n");
		goto err2;
	}

	demo_cls = class_create(THIS_MODULE,"hahamydemo");
	if(IS_ERR(demo_cls))
	{
		printk("class_create fail\n");
		ret = PTR_ERR(demo_cls);
		goto err3;
	}
	
	demo_devs = device_create(demo_cls,NULL,devno,NULL,"demo");
	if(IS_ERR(demo_devs))
	{
		printk("device_create fail\n");
		ret = PTR_ERR(demo_devs);
		goto err4;
	}
	
	sema_init(&sem1,1);
	sema_init(&sem2,0);
	printk("demo_init\n");
	return 0;
err4:
	class_destroy(demo_cls);
err3:
	cdev_del(demo_cdev);
err2:
	kfree(demo_cdev);
err1:
	unregister_chrdev_region(devno,1);
	return ret;
}

module_init(demo_init);

static void demo_exit(void)
{
	device_destroy(demo_cls,devno);

	class_destroy(demo_cls);
	cdev_del(demo_cdev);
	kfree(demo_cdev);
	unregister_chrdev_region(devno,1);
	printk("demo_exit\n");
	return ;
}
module_exit(demo_exit);
