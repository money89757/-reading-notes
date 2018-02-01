#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>

#include <asm/uaccess.h>

MODULE_LICENSE("GPL");

#define HELLO_MAJOR 260
#define HELLO_MINOR 0

static struct cdev cdev;
static char data[1024];

/*
 *  int (*open) (struct inode *, struct file *);
 */


static int hello_open(struct inode *inode, struct file *file)
{
	printk("hello open\n");
	return 0;
}

/*
 * int (*release) (struct inode *, struct file *);
 */
static int hello_release(struct inode *inode, struct file *file)
{
	printk("hello release\n");
	return 0;
}

/*
 * ssize_t (*write) (struct file *, const char __user *, size_t, loff_t *);
 */

static ssize_t hello_write(struct file *file, const char __user *buf, size_t count, loff_t *loff)
{
	if ((count > 1023) || (count < 0))	
		return -EINVAL;

	if (copy_from_user(data, buf, count))
		return -EFAULT;

	/*
	 * 将data中的数据写到设备中
	 */
	data[count] = '\0';
	printk("data = %s\n", data);

	return count;
}

/*
 *ssize_t (*read) (struct file *, char __user *, size_t, loff_t *);
 */
static ssize_t hello_read(struct file *file, char __user *buf, size_t count, loff_t *loff)
{
	if (count < 0)
		return -EFAULT;

	if (count > 1024)
		count = 1024;

	/*
	 * 从设备中将数据读到data中
	 */

	if (copy_to_user(buf, data, count))
		return -EFAULT;

	return count;
}

static long hello_ioctl (struct file *file, unsigned int  cmd, unsigned long  arg)
{


	printk("hello_ioctl  \n");
	printk("cmd  = %d,arg  = %d\n",cmd,arg);


	return  0;


}
static struct file_operations hello_fops = {
	.owner = THIS_MODULE,
	.open = hello_open,
	.release = hello_release,
	.write = hello_write,
	.read = hello_read,
	.unlocked_ioctl =  hello_ioctl,
};

static int hello_init(void)
{
	int ret;
	dev_t devno = MKDEV(HELLO_MAJOR, HELLO_MINOR);

	printk("Hello World\n");

	ret = register_chrdev_region(devno, 1, "hello");
	if (ret < 0) {
		printk("failed: register_chrdev_region\n");
		return ret;
	}

	/*
	 * void cdev_init(struct cdev *, const struct file_operations *);
	 */
	cdev_init(&cdev, &hello_fops);
	cdev.owner = THIS_MODULE;
	/*
	 * int cdev_add(struct cdev *, dev_t, unsigned);
	 */
	ret = cdev_add(&cdev, devno, 1);
	if (ret < 0) {
		printk("failed: cdev_add\n");
		goto err1;
	} 

	return 0;

err1:
	unregister_chrdev_region(devno, 1);
	return ret;
}

static void hello_exit(void)
{
	dev_t devno = MKDEV(HELLO_MAJOR, HELLO_MINOR);
	cdev_del(&cdev);
	unregister_chrdev_region(devno, 1);
	printk("Goodbye World\n");
}

module_init(hello_init);
module_exit(hello_exit);
