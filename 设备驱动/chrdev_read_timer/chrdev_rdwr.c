//头文件
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/string.h>
#include <linux/timer.h>
#include <linux/device.h>


#define GLOBALSIZE 128

//定义设备号变量
static unsigned baseminor = 0;
static unsigned count = 1;
const char * name = "jn16071";
static dev_t devno;


static struct scull_device {
	//定义cdev结构体
	struct cdev cdev;
	unsigned int second;
	
	struct class* my_class;
	struct timer_list timer;
	
}scull;

//自定义驱动方法open、release  和应用中的open和close对应
static int fs4412_driver_open(struct inode* inode, struct file* filp){
	//filp->private_data = &scull;
	filp->private_data = container_of(inode->i_cdev, struct scull_device,cdev);
	
	printk("fs4412_driver_open\n");
	return 0;
}
static int fs4412_driver_release(struct inode* inode, struct file* filp){

	struct scull_device* dev = filp->private_data;

	printk("fs4412_driver_release\n");
	return 0;
}

static ssize_t fs4412_driver_read(struct file* filp, char __user* buf, size_t cnt, loff_t* lops){


	put_user(scull.second,(unsigned int*)buf);
	//返回读取到的有效长度
	return sizeof(unsigned int);
}


static void fs4412_driver_timer(unsigned long args){

	mod_timer(&scull.timer,jiffies + HZ);
	scull.second +=1;
	printk("jiffies:%ld\n",jiffies);
}


static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = fs4412_driver_open,
	.release = fs4412_driver_release,
	.read = fs4412_driver_read,
	
};


static int fs4412_driver_init(void){
	int ret = 0;
	
	//获取设备号
	//注册设备号
	if(alloc_chrdev_region(&devno, baseminor,count, name)){
		printk("register_chrdev_region failed\n");
		goto err0;
	}
	scull.my_class = class_create(THIS_MODULE,"fs4412_class");
	device_create(scull.my_class,NULL,devno,NULL,"ex");
	//初始化cdev
	cdev_init(&scull.cdev,&fops);
	//向内核中添加cdev
	ret = cdev_add(&scull.cdev,devno,count);
	if(ret){
		printk("cdev_add failed,err:%d\n",ret);
		goto err1;
		
	}


	//初始化定义的timer_list
	init_timer(&scull.timer);
	scull.timer.function = fs4412_driver_timer;
	scull.timer.expires = jiffies + HZ;

	add_timer(&scull.timer);
	
	printk("devno:%d,major:%d,minor:%d\n",devno,MAJOR(devno),MINOR(devno));
	printk("fs4412_driver_init\n");
	return 0;
	
err1:	
	unregister_chrdev_region(devno,count);
err0:
	return 0;
}

static void fs4412_driver_exit(void){
	//释放资源
	del_timer(&scull.timer);
	cdev_del(&scull.cdev);
	unregister_chrdev_region(devno,count);
	
	device_destroy(scull.my_class,devno);
	class_destroy(scull.my_class);
	printk("Good Bye,linux kernel >_<\n");
}

//1. 入口修饰
module_init(fs4412_driver_init);

//2. 出口修饰

module_exit(fs4412_driver_exit);

//3. 许可证
MODULE_LICENSE("GPL");
