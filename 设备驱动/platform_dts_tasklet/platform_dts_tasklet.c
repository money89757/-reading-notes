//ͷ�ļ�
#include <linux/module.h>
#include <linux/init.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>
//����tasklet_struct
static struct tasklet_struct my_tasklet;

static const struct of_device_id fs4412_dts_table[] = {
	{.compatible = "jn16071,k2"},//���豸����һ�£�����ƥ���豸����Ϣ
	{/*��ֹԽ����ڱ�*/},
};
static irqreturn_t fs4412_handler(int irqno, void *dev){

	printk("handler irqno:%d\n",irqno);
	tasklet_schedule(&my_tasklet);
	return IRQ_HANDLED;
}
static void fs4412_after_handler_tasklet(unsigned long args){

	printk("fs4412_after_handler_tasklet\n");
}


static int fs4412_driver_probe(struct platform_device* pdev){
	//���������Դ��resource�ṹ��
	struct resource* r1 = NULL;
	r1 = platform_get_resource(pdev,IORESOURCE_IRQ,0);

	//�����ж�
	if(request_irq(r1->start,fs4412_handler,IRQF_DISABLED | IRQF_TRIGGER_FALLING,"jn16071-interrupt",NULL)){
		printk("request_irq failed\n");
		free_irq(r1->start,NULL);
		return -EFAULT;
	}

	tasklet_init(&my_tasklet,fs4412_after_handler_tasklet,0);
	printk("match ok\n");
	return 0;
}
static int fs4412_driver_remove(struct platform_device* pdev){
	struct resource* r1 = NULL;
	r1 = platform_get_resource(pdev,IORESOURCE_IRQ,0);
	//�ͷ�ռ�õ��жϺ�
	free_irq(r1->start,NULL);
	printk("fs4412_driver_remove\n");
	return 0;
}

//����platform_driver������Ϣ�����ṹ��
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
	//ע��platform_driver���ں���
	platform_driver_register(&pdrv);
	printk("Hello World ):\n");
	return 0;
}

static void fs4412_driver_exit(void){

	platform_driver_unregister(&pdrv);
	printk("Good Bye,linux kernel >_<\n");
}

//1. �������
module_init(fs4412_driver_init);

//2. ��������

module_exit(fs4412_driver_exit);

//3. ���֤
MODULE_LICENSE("GPL");
