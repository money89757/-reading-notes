//头文件
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/string.h>
#include <linux/platform_device.h>
#include <linux/of.h>

#include <asm/io.h>
#include "fs4412_ioctl.h"


#define GLOBALSIZE 128


//定义设备号变量
static unsigned major = 500;
static unsigned minor = 0;
static unsigned count = 1;
const char * name = "jn16071";
static dev_t devno;


static struct scull_device {
	char mem[GLOBALSIZE];
	int len;
	//定义cdev结构体
	struct cdev cdev;
	struct resource* led2_conrs;
	struct resource* led2_datrs;
	void __iomem* conaddr;
	void __iomem* dataddr;
	
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

	printk("scull.len: %d\n",dev->len);
	printk("fs4412_driver_release\n");
	return 0;
}
static long fs4412_driver_unlocked_ioctl(struct file* filp, unsigned int cmd, unsigned long args){

	if(_IOC_TYPE(cmd) != MAGIC_TYPE)

		return -EINVAL;
	
	switch(_IOC_NR(cmd)){
		case 0:{
			
			writel(readl(scull.dataddr) | (0x1 << 7) , scull.dataddr);
			printk("LED_ON\n");
			break;
		}
		case 1:{
			writel(readl(scull.dataddr) & ~(0x1 << 7) , scull.dataddr);
			printk("LED_OFF\n");
			break;
		}
		default:{
			break;
		}
	}
	
	return 0;
}

static int fs4412_driver_probe(struct platform_device * pdev){

	scull.led2_conrs = platform_get_resource(pdev,IORESOURCE_MEM,0);
	scull.led2_datrs = platform_get_resource(pdev,IORESOURCE_MEM,1);

	//映射寄存器物理地址到虚拟地址上
	scull.conaddr = ioremap(scull.led2_conrs->start,4);
	scull.dataddr = ioremap(scull.led2_datrs->start,4);

	writel((readl(scull.conaddr) & ~(0x0f << 28) ) | (0x1 << 28) , scull.conaddr);

	
	printk("match ok!\n");
	return 0;
}


static int fs4412_driver_remove(struct platform_device * pdev){

	//释放资源
	iounmap(scull.conaddr);
	iounmap(scull.dataddr);
	printk("fs4412_driver_remove\n");
	return 0;
}

static struct of_device_id fs4412_dts_table[] = {
	{.compatible = "fs4412,led2"},
	{},
}; 

static struct platform_driver pdrv = {
	.probe = fs4412_driver_probe,
	.remove = fs4412_driver_remove,
	.driver = {
		.name = "fs4412_driver",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(fs4412_dts_table),
	},
};
static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = fs4412_driver_open,
	.release = fs4412_driver_release,
	.unlocked_ioctl = fs4412_driver_unlocked_ioctl,
	
};


static int fs4412_driver_init(void){
	int ret = 0;
	
	//获取设备号
	devno = MKDEV(major,minor);
	//注册设备号
	if(register_chrdev_region(devno, count, name)){
		printk("register_chrdev_region failed\n");
		goto err0;
	}
	//初始化cdev
	cdev_init(&scull.cdev,&fops);
	//向内核中添加cdev
	ret = cdev_add(&scull.cdev,devno,count);
	if(ret){
		printk("cdev_add failed,err:%d\n",ret);
		goto err1;
		
	}

	//初始化自定义结构体scull
	memset(scull.mem,0,sizeof(scull.mem));
	scull.len = 0;

	platform_driver_register(&pdrv);
	printk("devno:%d,major:%d,minor:%d\n",devno,MAJOR(devno),MINOR(devno));
	printk("fs4412_driver_init\n");
	return 0;
	
err1:	
	unregister_chrdev_region(devno,count);
err0:
	return 0;
}

static void fs4412_driver_exit(void){
	
	platform_driver_unregister(&pdrv);
	cdev_del(&scull.cdev);
	unregister_chrdev_region(devno,count);
	printk("fs4412_driver_exit\n");
}

//1. 入口修饰
module_init(fs4412_driver_init);

//2. 出口修饰

module_exit(fs4412_driver_exit);

//3. 许可证
MODULE_LICENSE("GPL");
