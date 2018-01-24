//头文件
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>

//定义设备号变量
static unsigned baseminor = 0;
static unsigned count = 1;
const char * name = "jn16071";
static dev_t devno;
//定义cdev结构体
static struct cdev cdev;


struct class* my_class;


//自定义驱动方法open、release  和应用中的open和close对应
static int fs4412_driver_open(struct inode* inode, struct file* filp){

	printk("fs4412_driver_open\n");
	return 0;
}
static int fs4412_driver_release(struct inode* inode, struct file* filp){

	printk("fs4412_driver_release\n");
	return 0;
}

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = fs4412_driver_open,
	.release = fs4412_driver_release,
	
};


static int fs4412_driver_init(void){
	int ret = 0;
	
	//自动获取设备号//注册设备号
	if(alloc_chrdev_region(&devno, baseminor, count,name)){
		printk("register_chrdev_region failed\n");
		goto err0;
	}
	//初始化cdev
	cdev_init(&cdev,&fops);
	//向内核中添加cdev
	ret = cdev_add(&cdev,devno,count);
	if(ret){
		printk("cdev_add failed,err:%d\n",ret);
		goto err1;
		
	}
	//创建类class
	my_class = class_create(THIS_MODULE,"fs4412_class");
	device_create(my_class,NULL,devno,NULL,"ex");
	
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
	cdev_del(&cdev);
	unregister_chrdev_region(devno,count);

	device_destroy(my_class,devno);
	class_destroy(my_class);
	printk("Good Bye,linux kernel >_<\n");
}

//1. 入口修饰
module_init(fs4412_driver_init);

//2. 出口修饰

module_exit(fs4412_driver_exit);

//3. 许可证
MODULE_LICENSE("GPL");
