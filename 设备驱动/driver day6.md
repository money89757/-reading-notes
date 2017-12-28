# 模块三要素
MODULE_LICENES("GPL");  
模块加载：module_init(入口函数名);  
模块卸载：module_exit(出口函数名);  

##### Makefile
```C
ifeq ($(KERNELRELEASE),)
//KERNEL_DIR ?= /home/linux/linux-3.14
KERNEL_DIR ?= /lib/modules/$(shell uname -r)/build
PWD ?= $(shell pwd)

modules:
	make -C $(KERNEL_DIR) M=$(PWD) modules
clean:
	make -C $(KERNEL_DIR) M=$(PWD) clean
else

obj-m += xxx.o
endif
```

### 字符设备架构
```c
struct cdev mycdev;
char kbuf[1024];
int demo_open(struct inode *inode,struct file *filp)
{
	//获取私有数据
	//申请资源
	//计数
	return 0;
}

int demo_close(struct inode *inode,struct file *filp)
{
	//和open相反
	return 0;
}

//读：站在用户角度，数据从内核空间——>用户空间
//用户空间 read(fd,buf,sizeof(buf))
ssize_t demo_read(struct file *filp, char _user *ubuf, size_t size, loff_t *off)
{
	copy_to_user(ubuf,kbuf,用户空间需要最大缓存区大小);

	return 实际读取字节数;
}

write(fd,"hello",5);
ssize_t demo_write(struct file *filp, const char __user *ubuf, size_t size, loff_t *off)
{
	copy_from_user(kbuf,ubuf,size);
	return 实际写入的字节数;
}
//int a;
//int b = 10;
//ioctl(fd,cmd,&a);
//ioctl(fd,cmd,b);
long demo_ioctl(struct file *filep, unsigned int cmd, unsigned long arg)
{
	int kernel_a = 100;
	int kernel_b;
	switch(cmd)
	{
		case 宏1:
			copy_to_user(&arg,&kernel_a,sizeof(kernel_a));
		case 宏2:
			kernel_b = arg;
	}
}


struct file_operations ops = {
	.open = demo_open,
	.read = demo_read,
	.write = demo_write,
	.unlocked_ioctl = demo_ioctl，
	.release = demo_close,
};

struct class *cls;
struct device *devs;
入口函数内{
dev_t devno = MKDEV(主设备号,次设备号);

register_chrdev_region(起始设备号,注册的设备个数,名称)    /proc/devices和主设备号关联
cdev_init(&mycdev,&ops);
cdev_add(&cdev,注册的起始设备号，注册的设备个数);

cls = class_create(THIS_MODULE, "设备类的名字") //出现在/sys/class
devs = device_create(cls,NULL,devno,NULL,"demo");

}
出口函数{
从内核中删除字符设备,cdev_del(&cdev);
unregister_chrdev_region(起始设备号,需要注销的个数);
}
```

#### cmd命令无符号32位系统
```
8     8     2      	   14
类型   序号  方向   数据类型大小
```
---
### 内核的同步互斥机制
并发：宏观上同时运行，微观可能同时运行（多核），可能串行（针对可抢占操作系统）。
// 单核不可抢占  
单核可抢占  
//多核不可抢占  
多核可抢占  

---

单核CPU上进程和进程并发  
多CPU进程并发
进程和中断并发  
中断和中断并发  

#### 竞态：多个任务同时操作同一个共享空间，可能出现竞态

避免竞态：
* 中断屏蔽  

```
local_irq_disable();//关闭中断(关闭当前使用的cpu的中断)  
操作	//这部分代码叫做临界区，代码操作的资源叫做临界资源
local_irq_enable();//打开中断
操作系统的调度机制依赖中断。也就是说一旦某个cpu中断被关闭不会切换进程了。
其它cpu中断没有关闭，可以进行任务调度。
这种方式只适用于单核cpu。

```
* 原子操作
* 自旋锁
* 信号量
* 互斥锁
---

#### 互斥锁
```c
pthread_mutex_init();
pthread_mutex_lock();
```
初始化互斥锁：
```C
93 # define mutex_init(mutex) \                                                
94 do {                            \
95     static struct lock_class_key __key;     \
96                             \
97     __mutex_init((mutex), #mutex, &__key);      \
98 } while (0)
```
mutex的原型是 struct mutex *;  
使用方法：  
struct mutex mutex;  
mutex_init(&mutex);  
在驱动的模块入口函数中实现。

```
void mutex_lock(struct mutex *lock);
void mutex_unlock(struct mutex *lock);
```

## 自旋锁
自旋锁就是为了避免多核可抢占操作系统竞态。  
自旋锁机制；
```
任务
 ||
 \/
关闭抢占(任务不会轮询调度)
 ||
 \/			||——————————————————————
			\/					有 |
判断某个全局变量是否有人使用？——————
			||没有
			\/
		假设全局变量事先为1
		将全局变量设置为0
			||
			\/
		   工作(假设这里出现了中断，中断的内部也申请同一个自旋锁)
		    ||
			\/
		还原全局变量为1
			||
			\/
		 开启抢占

```

自旋锁如果条件不成立会移植占用cpu,互斥锁对比自旋锁CPU消耗很小。  
#### 自旋锁注意：
1. 不能在使用自旋锁期间出现睡眠类函数（死机）。
2. 在自旋锁使用期间如果使用了中断可能出现死锁。
3. 自旋锁使用过程中尽量保证操作系统时间足够短（上锁和解锁期间不要跨函数使用），临界区要短。
4. 互斥锁只能用于进程上下文，互斥锁使用过程中可以出现睡眠类的函数。  
自旋锁可以用于进程上下文也可以用于中断上下文。

```
295 #define spin_lock_init(_lock)               \
296 do {                            \
297     spinlock_check(_lock);              \                                   
298     raw_spin_lock_init(&(_lock)->rlock);        \
299 } while (0)

```
---

_lock原型是spinlock_t    
如何初始化自旋锁：  
spinlock_t lock;  
spin_lock_init(&lock);

---

如何上锁？  
static inline void spin_lock(spinlock_t *lock)  
使用时：spin_lock(&lock);

---

如何解锁？
static inline void spin_unlock(spinlock_t *lock)  
使用时：spin_unlock(&lock);  

---

### 信号量
应用成线程：
```C
sem_init();//初始化资源
sem_wait();//申请资源
sem_post();//释放资源
```

应用层锁机制：  
```C
pthread_mutex_init();
pthread_mutex_lock();
pthread_mutex_lock();

```

#### 无论是锁还是信号量（信号灯），都有一个共同特性--互斥

#### 互斥锁不关心顺序，信号量必须要保证执行顺序

### 内核中的信号量
```c
初始化信号量:sema_init();
申请资源:sema_down();
释放资源:sema_up();

```
## 阻塞
读阻塞或写阻塞  
读:数据从内核空间流向用户空间(如果没有数据，或者数据没有交给用户空间都是读阻塞)  

驱动读函数默认没有阻塞的，但是应用层函数有阻塞的。很多情况需要在驱动层接口中实现阻塞机制。  
如何在驱动中实现阻塞?使用等待队列  

### 等待队列使用机制
```C
定义等待队列头
  ||
  \/
初始化等待队列头
  ||
  \/
判断条件是否成立?（这里的条件我们自己任意定义的,假设a=10成立）
成立|| =======================
  \/                       ||
不会阻塞直接运行			 ||不成立
               \/
                阻塞  <=============================唤醒机制(使a=10)		

```

#### 涉及等待队列函数接口：
```C
70 #define init_waitqueue_head(q)              \
 71     do {                        \
 72         static struct lock_class_key __key; \
 73                             \
 74         __init_waitqueue_head((q), #q, &__key); \                           
 75     } while (0)

 13 void __init_waitqueue_head(wait_queue_head_t *q, const char *name, struct lo

```

#### 初始化等待对列头
```c

1、定义一个全局的 wait_queue_head_t q;
2、init_waitqueue_head(&q);
3、判断条件是否成立
	int a = 1;
	wait_event(q,a != 1); //当前这个函数会阻塞，等待其他的函数唤醒

4、唤醒函数  wake_up(&q);同时还要使条件成立a = 0;

```
---

```
244 #define wait_event(wq, condition)                   \
245 do {                                    \
246     if (condition)                          \
247         break;                          \
248     	__wait_event(wq, condition); 条件不成立执行当前函数                               
249 } while (0)   ||
				  ||
				  \/
	228 #define __wait_event(wq, condition)                 \                       
	229     (void)___wait_event(wq, condition, TASK_UNINTERRUPTIBLE, 0, 0,  \
	230                 schedule())
						||
						\/
			200     INIT_LIST_HEAD(&__wait.task_list);  初始化了一个链表头
			201     if (exclusive)                          \
			202         __wait.flags = WQ_FLAG_EXCLUSIVE;           \
			203     else                                \
			204         __wait.flags = 0;    赋值flags为0
			205                                     \
			206     for (;;) {                          \
			207         long __int = prepare_to_wait_event(&wq, &__wait, state);存放进程信息到链表中；wait->func = autoremove_wake_function;这是一个函数入口         
			208                                     \
			209         if (condition)   当prepare_to_wait_event执行完成后condition条件可能成立
			210             break;                      \
			211                                     \
			212         if (___wait_is_interruptible(state) && __int) {     \
			213             __ret = __int;                  \
			214             if (exclusive) {                \
			215                 abort_exclusive_wait(&wq, &__wait,
			216                              state, NULL);  \
			217                 goto __out;             \
			218             }                       \
			219             break;                      \
			220         }                           \
			221                                     \
			222         cmd; //schedule()这个函数如果操作了TASK_INTERRUPTIBLE 或者 TASK_UNINTERRUPTIBLE 会使进程睡眠
			223     }                               \
			224     finish_wait(&wq, &__wait);                  \
			225 __out:  __ret;                              \
			226 })

```

#### wake_up()函数分析
```
159 #define wake_up(x)          __wake_up(x, TASK_NORMAL, 1, NULL)
								||
								\/
					94     __wake_up_common(q, mode, nr_exclusive, 0, key); 			
								||
								\/
						curr->func(curr, mode, wake_flags, key)  
							==》调用wait_event内部的autoremove_wake_function
														||
														\/
													设置状态为TASK_WAKING


```
