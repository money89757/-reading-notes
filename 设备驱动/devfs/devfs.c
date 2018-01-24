//ͷ�ļ�
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>

//�����豸�ű���
static unsigned baseminor = 0;
static unsigned count = 1;
const char * name = "jn16071";
static dev_t devno;
//����cdev�ṹ��
static struct cdev cdev;


struct class* my_class;


//�Զ�����������open��release  ��Ӧ���е�open��close��Ӧ
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
	
	//�Զ���ȡ�豸��//ע���豸��
	if(alloc_chrdev_region(&devno, baseminor, count,name)){
		printk("register_chrdev_region failed\n");
		goto err0;
	}
	//��ʼ��cdev
	cdev_init(&cdev,&fops);
	//���ں������cdev
	ret = cdev_add(&cdev,devno,count);
	if(ret){
		printk("cdev_add failed,err:%d\n",ret);
		goto err1;
		
	}
	//������class
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
	//�ͷ���Դ
	cdev_del(&cdev);
	unregister_chrdev_region(devno,count);

	device_destroy(my_class,devno);
	class_destroy(my_class);
	printk("Good Bye,linux kernel >_<\n");
}

//1. �������
module_init(fs4412_driver_init);

//2. ��������

module_exit(fs4412_driver_exit);

//3. ���֤
MODULE_LICENSE("GPL");
