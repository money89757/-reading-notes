#include<linux/fs.h>
#include<linux/module.h>
#include<linux/init.h>
#include<linux/cdev.h>
#include<linux/slab.h>
#include<linux/device.h>
#include<asm/uaccess.h>

#define DEMO_MAJOR 255
#define DEMO_MINOR 0

dev_t devno = MKDEV(DEMO_MAJOR,DEMO_MINOR);
//struct cdev *demo_cdev;

struct class *demo_cls;
struct device *demo_devs;

int count = 3;
int major = 0;

//char kbuf[1024];
//int len;

struct test_cdev 
{
	struct cdev demo_cdev;
	char kbuf[1024];
	int len;
};

struct test_cdev *gcd;

MODULE_LICENSE("GPL");

static int demo_open(struct inode *inode,struct file *filp)
{
	struct test_cdev *cd;
	cd = container_of(inode->i_cdev,struct test_cdev,demo_cdev);
	filp->private_data = cd;
	cd->len = 0;
	printk("demo_open\n");
	return 0;
}
static ssize_t demo_read(struct file *filp,char __user *ubuf,size_t size,loff_t *off)
{
	struct test_cdev *cd;
	int n;
	int ret;
	char *tmp;
	cd = filp->private_data;
	if(*off == cd->len)
		*off = 0;

	if(size > cd->len - *off)
		n = cd->len - *off;
	else 
		n = size;
	tmp = cd->kbuf + *off; //每次读取时的起始地址
	ret = copy_to_user(ubuf,tmp,n);
	if(ret != 0)
	{
		printk("copy_to_user fail\n");
		return -EFAULT;
	}

	*off += n;
	printk("demo_read success\n");
	return n;
}

static ssize_t demo_write(struct file *filp,const char __user *ubuf,size_t size,loff_t *off)
{
	struct test_cdev *cd;
	int ret;
	int n;
	char *tmp;

	cd = filp->private_data;

	if(*off == sizeof(cd->kbuf))
		return -ENOMEM;
	if(size > sizeof(cd->kbuf) - *off)
		n = sizeof(cd->kbuf) - *off;
	else 
		n = size;
	tmp = cd->kbuf + *off;
	
	ret = copy_from_user(tmp,ubuf,n);
	if(ret != 0)
	{
		printk("copy_from_user fail\n");
		return -EFAULT;
	}

	*off += n;
	cd->len += n;//控制一共写入的字节数	
	printk("driver write:len = %d,kbuf:%s\n",cd->len,cd->kbuf);
	printk("demo_write success\n");
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
	int i;
	ret = register_chrdev_region(devno,count,"demo");
	if(ret != 0)
	{
		printk("register_chrdev_region fail\n");
		ret = alloc_chrdev_region(&devno,0,count,"demo");
		if(ret != 0)
		{
			printk("alloc_chrdev_region fail\n");
			return -ENOMEM;
		}
	}
	
	major = MAJOR(devno);

	gcd = kzalloc(count * sizeof(struct test_cdev),GFP_KERNEL);
	if(IS_ERR(gcd)) //返回值为非0代表出错
	{
		ret = PTR_ERR(gcd);
		goto err1;
	}
	
	for(i = 0;i < count;i ++)
	{
		cdev_init(&gcd[i].demo_cdev,&demo_ops);
		ret = cdev_add(&gcd[i].demo_cdev,MKDEV(major,i),1);
		if(ret != 0)
		{
			printk("cdev_add fail\n");
			goto err2;
		}
	}
	demo_cls = class_create(THIS_MODULE,"hahamydemo");
	if(IS_ERR(demo_cls))
	{
		printk("class_create fail\n");
		ret = PTR_ERR(demo_cls);
		goto err3;
	}
	
	for(i = 0;i < count;i ++)
	{
		demo_devs = device_create(demo_cls,NULL,MKDEV(major,i),NULL,"demo%d",i);
		if(IS_ERR(demo_devs))
		{
			printk("device_create fail\n");
			ret = PTR_ERR(demo_devs);
			goto err4;
		}
	}
	printk("demo_init\n");
	return 0;
err4:
	class_destroy(demo_cls);
err3:
	for(i = 0;i < 3;i ++)
	{
		cdev_del(&gcd[i].demo_cdev);
	}
err2:
	kfree(gcd);
err1:
	unregister_chrdev_region(devno,3);
	return ret;
}

module_init(demo_init);

static void demo_exit(void)
{
	int i;
	for(i = 0;i < count;i ++)
	{
		device_destroy(demo_cls,MKDEV(major,i));
	}
	class_destroy(demo_cls);
	for(i = 0;i < count;i ++)
		cdev_del(&gcd[i].demo_cdev);
	kfree(gcd);
	unregister_chrdev_region(devno,count);
	printk("demo_exit\n");
	return ;
}
module_exit(demo_exit);
