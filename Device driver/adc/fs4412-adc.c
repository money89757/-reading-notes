#include<linux/init.h>
#include<linux/module.h>
#include<linux/platform_device.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/slab.h>
#include<asm/io.h>
#include<asm/uaccess.h>
#include<linux/wait.h>
#include<linux/sched.h>
#include<linux/interrupt.h>

#define ADC_MAJOR 500
#define ADC_MINOR 0

#define ADCMUX  0x1C
#define ADCCLRINT 0x18
#define ADCDAT  0x0C
struct fs4412_adc 
{
	dev_t devno;
	struct cdev fs4412_cdev;
	struct class *fs4412_cls;
	struct device *fs4412_devs;
	struct resource *res_mem;
	struct resource *res_irq;
	void *adc_base;//注意一定要定义为void *类型
	wait_queue_head_t readq;
	int flag;
};
struct fs4412_adc *myadc;

int fs4412_adc_open(struct inode *inode,struct file *filp)
{
	return 0;
}

ssize_t fs4412_adc_read(struct file *filp,char __user *ubuf,size_t size,loff_t *off)
{
	int data;
	int ret;
	writel(readl(myadc->adc_base) | 1,myadc->adc_base);
	wait_event_interruptible(myadc->readq,myadc->flag != 0);	

	data = readl(myadc->adc_base + 0x0C) & 0xfff;
	ret = copy_to_user(ubuf,&data,size);
	if(ret)
		return -EINVAL;
	myadc->flag = 0;
	return size;
}

int fs4412_adc_close(struct inode *inode,struct file *filp)
{
	return 0;
}

irqreturn_t fs4412_adc_handler(int irqno,void *dev)
{
	myadc->flag = 1;
	writel(0,myadc->adc_base + ADCCLRINT);
	wake_up_interruptible(&myadc->readq);
	return IRQ_HANDLED;
}

struct file_operations fs4412_adc_ops = {
	.open = fs4412_adc_open,
	.release = fs4412_adc_close,
	.read = fs4412_adc_read,
};

int fs4412_adc_probe(struct platform_device *fs4412_pdev)
{
	int ret;
	printk("match ok\n");
	
	myadc = kzalloc(sizeof(struct fs4412_adc),GFP_KERNEL);	
	if(myadc == NULL)
	{
		printk("kzalloc fail\n");
		return -ENOMEM;
	}

	myadc->devno = MKDEV(ADC_MAJOR,ADC_MINOR);

	ret = register_chrdev_region(myadc->devno,1,"fs4412-adc");
	if(ret != 0)
	{
		printk("register_chrdev_region fail\n");
		goto err1;
	}

	cdev_init(&myadc->fs4412_cdev,&fs4412_adc_ops);
	cdev_add(&myadc->fs4412_cdev,myadc->devno,1);

	myadc->fs4412_cls = class_create(THIS_MODULE,"adc");
	if(IS_ERR(myadc->fs4412_cls))
	{
		ret = PTR_ERR(myadc->fs4412_cls);
		goto err2;
	}

	myadc->fs4412_devs = device_create(myadc->fs4412_cls,NULL,myadc->devno,NULL,"adc");
	if(IS_ERR(myadc->fs4412_devs))
	{
		ret = PTR_ERR(myadc->fs4412_devs);
		goto err3;
	}

	myadc->res_mem = platform_get_resource(fs4412_pdev,IORESOURCE_MEM,0);
	if(myadc->res_mem == NULL)
	{
		printk("platform_get_resource mem fail\n");
		return -EINVAL;
	}

	myadc->adc_base = ioremap(myadc->res_mem->start,myadc->res_mem->end - myadc->res_mem->start);
	if(myadc->adc_base == NULL)
	{
		printk("ioremap fail\n");
		return -EINVAL;
	}

	writel(1 << 16 | 1 << 14 | 0xff << 6,myadc->adc_base);
	writel(3,myadc->adc_base + ADCMUX);
	
	myadc->res_irq = platform_get_resource(fs4412_pdev,IORESOURCE_IRQ,0);
	if(myadc->res_irq == NULL)
	{
		printk("platform_get_resource irq fail\n");
		return -EINVAL;
	}

	ret = request_irq(myadc->res_irq->start,fs4412_adc_handler,IRQF_DISABLED,"fs4412-adc",NULL);
	
	init_waitqueue_head(&myadc->readq);
	printk("fs4412_adc_probe\n");
	return 0;
err3:
	class_destroy(myadc->fs4412_cls);
err2:
	cdev_del(&myadc->fs4412_cdev);
	unregister_chrdev_region(myadc->devno,1);
err1:
	kfree(myadc);
	return ret;
}

int fs4412_adc_remove(struct platform_device *fs4412_pdev)
{
	device_destroy(myadc->fs4412_cls,myadc->devno);
	class_destroy(myadc->fs4412_cls);
	cdev_del(&myadc->fs4412_cdev);
	unregister_chrdev_region(myadc->devno,1);
	kfree(myadc);
	printk("fs4412_adc_remove\n");
	return 0;
}

struct of_device_id fs4412_adc_of_table[] = {
	{
		.compatible = "fs4412,adc",
	},
	{},
};

struct platform_driver fs4412_adc_drv = {
	.driver = {
		.name = "fs4412-adc",
		.of_match_table = fs4412_adc_of_table,
	},

	.probe = fs4412_adc_probe,
	.remove = fs4412_adc_remove,
};
module_platform_driver(fs4412_adc_drv);
#if 0
int fs4412_adc_init(void)
{
	platform_driver_register(&fs4412_adc_drv);
	printk("fs4412_adc_init\n");
	return 0;
}
module_init(fs4412_adc_init);

void fs4412_adc_exit(void)
{
	platform_driver_unregister(&fs4412_adc_drv);
	printk("fs4412_adc_exit\n");
	return;
}
module_exit(fs4412_adc_exit);
#endif

MODULE_LICENSE("GPL");
