//头文件
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/string.h>
#include <linux/delay.h>
#include <linux/sched.h>
#include <linux/poll.h>




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
	wait_queue_head_t rq,wq;
	struct semaphore sem;
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

static ssize_t fs4412_driver_read(struct file* filp, char __user* buf, size_t cnt, loff_t* lops){
	struct scull_device* dev = filp->private_data;
	//判断有效数据长度
	if(cnt < 0)return -EINVAL;

	down_interruptible(&scull.sem);
	//对资源有效长度的判断
	if(dev->len == 0){
		up(&scull.sem);
		if(filp->f_flags & O_NONBLOCK){
			//非阻塞
			printk("Can't read: busy\n");
			return -EAGAIN;
		}else{
			//阻塞
			//msleep(10);
			wait_event_interruptible(scull.rq,(dev->len != 0));
		}
		down_interruptible(&scull.sem);
	}
	
	if(cnt > dev->len)cnt = dev->len;
	//copy_to_user 失败时，返回值是不能读到的字节数，成功返回0
	if(!copy_to_user(buf,dev->mem,cnt)){
		//擦除已经读取的数据
		strcpy(dev->mem,dev->mem + cnt);
		//更新，减有效数据长度的值
		dev->len -= cnt;
	}else{
		printk("copy_to_user failed\n");
		return -EINVAL;
	}

	up(&scull.sem);
	
	wake_up_interruptible(&scull.wq);
	
	//返回读取到的有效长度
	return cnt;
}

static ssize_t fs4412_driver_write(struct file* filp,const char __user* buf, size_t cnt, loff_t* lops){
	struct scull_device* dev = filp->private_data;
	
	//判断有效数据长度
	if(cnt < 0)return -EINVAL;

	down_interruptible(&scull.sem);
	//对资源有效长度的判断
	if(dev->len == GLOBALSIZE){
		up(&scull.sem);
		if(filp->f_flags & O_NONBLOCK){
			//非阻塞
			printk("Can't write: busy\n");
			return -EAGAIN;
		}else{
			//阻塞
			//msleep(10);
			wait_event_interruptible(scull.wq,(dev->len != GLOBALSIZE));
		}
		down_interruptible(&scull.sem);
	}

	if(cnt > GLOBALSIZE - dev->len)cnt = GLOBALSIZE - dev->len;
	//copy_from_user 失败时，返回值是不能写的字节数，成功返回0
	if(copy_from_user(dev->mem + dev->len, buf,cnt)){
		printk("copy_from_user failed\n");
		return -EFAULT;
	}else{
		dev->len += cnt;
	}

	up(&scull.sem);
	
	wake_up_interruptible(&scull.rq);
	
	printk("dev->mem:%s\n",dev->mem);
	return cnt;
}

unsigned int fs4412_driver_poll(struct file* filp, struct poll_table_struct* wait){

	unsigned int mask = 0;
	struct scull_device* dev = filp->private_data;
	
	//1. 向poll_table_struct中添加等待队列头
	poll_wait(filp,&dev->rq,wait);
	poll_wait(filp,&dev->wq,wait);
	
	//2. 判断资源状态，返回响应的掩码
	//可读
	if(dev->len != 0){
		mask |= POLLIN | POLLRDNORM;
	}
	//可写
	if(dev->len != GLOBALSIZE){
		mask |= POLLOUT | POLLWRNORM;
	}
	
	return mask;	
}


static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = fs4412_driver_open,
	.release = fs4412_driver_release,
	.read = fs4412_driver_read,
	.write = fs4412_driver_write,
	.poll = fs4412_driver_poll,
	
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

	//初始化读写等待队列头
	init_waitqueue_head(&scull.rq);
	init_waitqueue_head(&scull.wq);
	
	//初始化信号量
	sema_init(&scull.sem,1);
	
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
	cdev_del(&scull.cdev);
	unregister_chrdev_region(devno,count);
	printk("Good Bye,linux kernel >_<\n");
}

//1. 入口修饰
module_init(fs4412_driver_init);

//2. 出口修饰

module_exit(fs4412_driver_exit);

//3. 许可证
MODULE_LICENSE("GPL");
