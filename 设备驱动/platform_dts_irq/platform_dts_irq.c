//头文件
#include <linux/module.h>
#include <linux/init.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>


static const struct of_device_id fs4412_dts_table[] = {
	{.compatible = "jn16071,k2"},//和设备树中一致，才能匹配设备树信息
	{/*防止越界的哨兵*/},
};
static irqreturn_t fs4412_handler(int irqno, void *dev){

	printk("handler irqno:%d\n",irqno);
	return IRQ_HANDLED;
}

static int fs4412_driver_probe(struct platform_device* pdev){
	//定义接受资源的resource结构体
	struct resource* r1 = NULL;
	r1 = platform_get_resource(pdev,IORESOURCE_IRQ,0);

	//申请中断
	if(request_irq(r1->start,fs4412_handler,IRQF_DISABLED | IRQF_TRIGGER_FALLING,"jn16071-interrupt",NULL)){
		printk("request_irq failed\n");
		free_irq(r1->start,NULL);
		return -EFAULT;
	}
	
	printk("match ok\n");
	return 0;
}
static int fs4412_driver_remove(struct platform_device* pdev){
	struct resource* r1 = NULL;
	r1 = platform_get_resource(pdev,IORESOURCE_IRQ,0);
	//释放占用的中断号
	free_irq(r1->start,NULL);
	printk("fs4412_driver_remove\n");
	return 0;
}

//定义platform_driver驱动信息描述结构体
static struct platform_driver pdrv = {
	.probe = fs4412_driver_probe,
	.remove = fs4412_driver_remove,
	.driver = {
		.name = "fs4412_device",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(fs4412_dts_table),
	},
};

static int fs4412_driver_init(void){
	//注册platform_driver到内核中
	platform_driver_register(&pdrv);
	printk("Hello World ):\n");
	return 0;
}

static void fs4412_driver_exit(void){

	platform_driver_unregister(&pdrv);
	printk("Good Bye,linux kernel >_<\n");
}

//1. 入口修饰
module_init(fs4412_driver_init);

//2. 出口修饰

module_exit(fs4412_driver_exit);

//3. 许可证
MODULE_LICENSE("GPL");
