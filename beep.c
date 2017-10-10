#include <linux/module.h>  
#include <linux/fs.h>  
#include <linux/cdev.h>  
#include <linux/device.h>  
#include <asm/io.h>  
#include <asm/uaccess.h>  
#include <linux/miscdevice.h>
#include <linux/list.h>
#include <linux/platform_device.h>
#include <linux/types.h>

#define DRIVER_NAME "beep_demo"

#define GPD0CON       0x114000a0  
#define TIMER_BASE    0x139D0000             
#define TCFG0         0x0000                 
#define TCFG1         0x0004                              
#define TCON          0x0008               
#define TCNTB0        0x000C            
#define TCMPB0        0x0010             

static unsigned int *gpd0con;  
static void *timer_base; 
static struct resource *res1;
static struct resource *res2;

#define  MAGIC_NUMBER    'k'  
#define  BEEP_ON    _IO(MAGIC_NUMBER    ,0)  
#define  BEEP_OFF   _IO(MAGIC_NUMBER    ,1)  
#define  BEEP_FREQ   _IO(MAGIC_NUMBER   ,2)  

static void beep_init(void)  
{  
	gpd0con = ioremap(GPD0CON,4);  
	timer_base = ioremap(TIMER_BASE,0x14);  

	writel ((readl(gpd0con)&~(0xf<<0)) | (0x2<<0),gpd0con);  
	writel ((readl(timer_base +TCFG0  )&~(0xff<<0)) | (0xff <<0),timer_base +TCFG0);   
	writel ((readl(timer_base +TCFG1 )&~(0xf<<0)) | (0x2 <<0),timer_base +TCFG1 );   

	writel (500, timer_base +TCNTB0  );  
	writel (250, timer_base +TCMPB0 );  
	writel ((readl(timer_base +TCON )&~(0xf<<0)) | (0x2 <<0),timer_base +TCON );   
}  

void beep_on(void)  
{  
	writel ((readl(timer_base +TCON )&~(0xf<<0)) | (0x9 <<0),timer_base +TCON );  
}  

void beep_off(void)  
{  
	writel ((readl(timer_base +TCON )&~(0xf<<0)) | (0x0 <<0),timer_base +TCON );  
}  


static int beep_open (struct inode *inode, struct file *filep)  
{  
	beep_on();  
	return 0;  
}  

static int beep_release(struct inode *inode, struct file *filep)  
{  
	beep_off();  
	return 0;  
}  

static void beep_unmap(void)
{
	iounmap(gpd0con);
	iounmap(timer_base);
}

#define BEPP_IN_FREQ 100000  
static void beep_freq(unsigned long arg)  
{  
	writel(BEPP_IN_FREQ/arg, timer_base +TCNTB0  );  
	writel(BEPP_IN_FREQ/(2*arg), timer_base +TCMPB0 );  

}  

static long beep_ioctl(struct file *filep, unsigned int cmd, unsigned long arg)  
{  
	switch(cmd)  
	{  
	case BEEP_ON:  
		beep_on();  
		break;  
	case BEEP_OFF:  
		beep_off();  
		break;  
	case BEEP_FREQ:  
		beep_freq( arg );  
		break;  
	default :
		break;
		return -EINVAL;  
	}  
}  

static struct file_operations beep_fops=  
{  
	.open     = beep_open,  
	.release = beep_release,  
	.unlocked_ioctl      = beep_ioctl,  
};  

static struct miscdevice misc_dev = {

	.minor = MISC_DYNAMIC_MINOR,
	.name = DRIVER_NAME,
	.fops = &beep_fops,

};

static int beep_probe(struct platform_device *beep_dev)
{
	int ret;
	printk("match OK\n");
	ret = misc_register(&misc_dev);
	if(ret < 0)
	{
		printk("misc_register is fail\n");	
		return -EBUSY;
	}
	
	res1 = platform_get_resource(beep_dev,IORESOURCE_MEM,0);
	res2 = platform_get_resource(beep_dev,IORESOURCE_MEM,1);

	gpd0con = ioremap(res1->start,res1->end-res1->start);
	
	timer_base = ioremap(res2->start,res2->end-res2->start);

	beep_init();
	return 0;
}

static int beep_remove(struct platform_device *beep_dev)
{
	misc_deregister(&misc_dev);
	beep_unmap();
	return 0;
}

static struct of_device_id beep_id_table[] = {

	{
		.compatible = "fs4412,beep",
	},
};

static struct platform_driver beep_driver = {
	

	.probe = beep_probe,
	.remove = beep_remove,
	
	.driver = {
		.name = "beep",
		.of_match_table = beep_id_table,
	},

};

static int __init beep_misc_init(void)
{
	int ret;
	ret = platform_driver_register(&beep_driver);
	if(ret < 0)
	{
		printk("platform_driver_register\n");
		return ret;
	}
	printk("beep_misc_init\n");
	return 0;
}


static void __exit beep_misc_exit(void)
{
	printk("beep_misc_exit\n");
	platform_driver_unregister(&beep_driver);
	return ;
}
      
MODULE_LICENSE("GPL");  
module_init(beep_misc_init);  
module_exit(beep_misc_exit);

