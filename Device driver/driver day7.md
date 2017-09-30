## 同步互斥
用户需要高效率，所以CPU以及操作系统有并发。  
因为并发的存在所以会出现竞态。  

### 避免竞态的产生
中断屏蔽  
  关闭中断  
  临界区(本身就是代码)->代码操作的资源叫做理解资源  
  开启中断
只适用于单核cpu  

### 原子操作
```c
原子操作
atomic_t v;
初始化原子操作:atomic_set(&v,1);

atomic_dec_and_test();

demo_open()
{
	if(!atomic_dec_and_test(&v))
	{
		atomic_inc(&v);
		return -EBUSY;
	}
	正常打开
}

demo_close()
{
	atomic_inv();
}

```

## 互斥锁
```C
mutex_init();
mutex_lock();
mutex_unlock();
只是完成互斥功能，只能用于进程上下文，上锁和解锁的内部操作中可以有延时类函数

```

## 自旋锁
```c
spin_lock_init();
spin_lock();
spin_unlock();

```

## 信号量
```c
sema_init();
down(); //down_interruptible();
up();

```

## 等待队列
```c
nt flag = 1;
定义等待队列头信息  wait_queue_head_t q;
	||
	\/
初始化等待队列头  init_waitqueue_head(&q);使用于模块入口函数中
	||
	\/
阻塞，通过条件判断是否将进程信息添加到等待队列中。wait_event(q,flag != 1);flag = 0
	||
	\/
   阻塞 《==============================wake_up(&q);flag = 1;

wait_event();
wake_up();

wait_event_interruptible();
wake_up_interruptibel();

```

# 应用层多路复用
```C
int select(int nfds, fd_set *readfds, fd_set *writefds,
                  fd_set *exceptfds, struct timeval *timeout);

功能:如果3张表中有任意一个文件描述符就绪select会执行，如果没有任何文件描述符就绪会阻塞。
	 如果有就绪的文件描述符，会清除没有就绪的文件描述符。
参数1:最大文件描述符+1
参数2:读类文件描述符表
参数3:写类....
参数4:异常类.........
参数5:超时检测

FD_ZERO();清零文件描述符表
FD_SET();在文件描述符表中添加新的文件描述符
FD_ISSET();判断文件描述符是否在表中。
FD_CLR();从文件描述符表中删除文件描述符

```

# 内核poll机制
```C
#define POLLIN      0x0001  数据携带优先级可读
#define POLLRDNORM  0x0040  普通数据可读

#define POLLOUT     0x0004
#define POLLWRNORM  0x0100

```

## 应用层的select对应了驱动层的poll接口
```C
vim arch/arm/include/uapi/asm/unistd.h
10 #define __NR_select         (__NR_SYSCALL_BASE+ 82)
				||
				\/
827 #define __NR_select 1067
828 #define __ARCH_WANT_SYS_SELECT
829 __SYSCALL(__NR_select, sys_select)
								||
								\/
					640     ret = core_sys_select(n, inp, outp, exp, to);
										||
										\/
								599     ret = do_select(n, &fds, end_time);
												||
												\/
									 402     struct poll_wqueues table;
									 403     poll_table *wait;

									 417     poll_initwait(&table);
									435         for (i = 0; i < n; ++rinp, ++routp, ++rexp) {

											460     mask = (*f_op->poll)(f.file, wait);
											//调用了驱动层的poll函数，我们自己的驱动层中需要返回掩码

											 463                     if ((mask & POLLIN_SET) && (in & bit)) { 这一步执行之前保证文件描述符就绪。
											 464                         res_in |= bit;
											 465                         retval++; 统计读类就绪的文件描述符个数
											 466                         wait->_qproc = NULL;
											 467                     }

									}


poll_initwait(&table);
	||
	\/
121     init_poll_funcptr(&pwq->pt, __pollwait);
										||
										\/
 219 static void __pollwait(struct file *filp, wait_queue_head_t *wait_address,
 220                 poll_table *p)
 221 {
 222     struct poll_wqueues *pwq = container_of(p, struct poll_wqueues, pt);
 223     struct poll_table_entry *entry = poll_get_entry(pwq);
 224     if (!entry)
 225         return;
 226     entry->filp = get_file(filp);
 227     entry->wait_address = wait_address; //wait_address应该是我们自己在驱动层实现的一个等待队列头
 228     entry->key = p->_key;
 229     init_waitqueue_func_entry(&entry->wait, pollwake);
 230     entry->wait.private = pwq;
 231     add_wait_queue(wait_address, &entry->wait); //等待队列头加到等待队列中
 232 }

```
注意： 注意:wait_address需要我们自己实现后传递给内核的。内核中接口名称__pollwait  
内核提示了，如果需要使用__pollwait功能，通过调用内核中的poll_wait();  
42 static inline void poll_wait(struct file * filp, wait_queue_head_t * wait_address, poll_table *p)  

##### 自己驱动中：
```C
unsigned int demo_poll(struct file *filp, struct poll_table_struct *poll_table)
{
	poll_wait(filp,自己定义初始化的等待队列头,poll_table);

	实现mask
	return mask;
}

poll_table的结构体指针，在内核中已经定义好了这个结构体空间

```

# 中断机制：
1. 硬件中断
```
a. 可以被分为内部中断（相当于ARM讲过的异常）和外部中断（底板一些外设的中断）  
b. 向量中断（一个中断对应一个入口）和非向量中断（中断对应的入口）
c. 可屏蔽中断或者不可屏蔽中断
```
2. 硬件上中断的使用原理
```
必须了解外设、中断控制器、CPU之间的关系
```
3. 中断号

```

SPI port No      ID
PPI 私有中断   中断号只能交给一个指定cpu
SPI 共享中断   中断号可以发送给一组cpu
SPI或者PPI操作的中断都是外设中断。

SGI 软中断     CPU和CPU之间进行交互的。

HW interrupt nuber硬件中断号,在使用时使用的是SPI port No
不同的中断控制器可能操作的硬件中断号是相同的。
irq number 虚拟中断号  给cpu识别的。
硬件中断号和虚拟中断号之间要做映射。映射是内核实现的。

```
内核中断子系统原理

```c
第一步:初始化中断控制器相关的寄存器，内核中一旦初始化成功会产生一个irq_chip结构体(描述中断控制器)


第二步:注册中断到内核中(让系统认识中断号，让系统知道如果出现中断去哪里运行——中断处理函数)
---
static inline int __must_check request_irq(unsigned int irq, irq_handler_t handler, unsigned long flags,const char *name, void *dev)
功能:注册硬件产生的中断到内核
参数1:硬件中断号
参数2:中断处理函数的入口
参数3:中断类型以及触发方式
	对于中断类型:
	61 #define IRQF_DISABLED       0x00000020   使用的快中断，屏蔽除了自己之外其它所有中断
	62 #define IRQF_SHARED     0x00000080   说明我们的外设使用共享中断
	对于中断触发方式:
	上升沿触发:IRQF_TRIGGER_RISING
	下降沿触发:IRQF_TRIGGER_FALLING
	高电平触发:IRQF_TRIGGER_HIGH
	低电平触发:IRQF_TRIGGER_LOW
	使用方法:IRQF_DISABLED | IRQF_TRIGGER_LOW
参数4:名称——使中断号信息和名称绑定，存放在/proc/interrupts
参数5:给中断处理函数传参的。当外部中断使用方式为SPI时可能使用到.
---

第三步:操作硬件使中断产生，进而调用中断处理函数。

马上要写驱动实现注册以及调用中断处理函数。需要知道硬件信息,如果我们编写按键驱动

```


### 如何写出相关驱动
1. 查看板子原理图

```
底板寻找 UART_RING  ---> 核心板 XEINT9/KP_COL1/ALV_DBG5/GPX1_1         UART_RING
UART_RING链接到了核心板GPX1  gpio控制器上，同时又发现XEINT9,也就是说我们当前并不是单纯使用gpio控制器
可能需要使用gpx1中断控制器

```

2. vi arch/arm/boot/dts/exynos4412-fs4412.dts

```c
寻找gpx1标号或者节点名称，找不到，最终vi arch/arm/boot/dts/exynos4x12-pinctrl.dts中找到
571         gpx1: gpx1 {
572             gpio-controller;
573             #gpio-cells = <2>;
574
575             interrupt-controller;//说明这是一个中断控制器
576             interrupt-parent = <&gic>;//继承了gic中断控制器一些属性。
577             interrupts = <0 24 0>, <0 25 0>, <0 26 0>, <0 27 0>,
578                      <0 28 0>, <0 29 0>, <0 30 0>, <0 31 0>;
579             #interrupt-cells = <2>; //如果有外设节点接在gpx1中断控制器下，那么在外设设备树节点中的interrupts属性有两个值
580         };

针对于普通的中断控制器中interrupts = <0 24 0>值如果有三部分:
   vi Documentation/devicetree/bindings/arm/gic.txt
   第一部分:中断类型
   0代表共享中断
   1代表私有中断

   第二部分:硬件中断号

   第三部分:中断触发方式
			1上升沿
			2下降沿
			4高电平
			8低电平
   如果值为0说明暂时不关心中断触发方式，这个值由外设节点传过来的。

   如果interrupts有两个值:
   interrupts = <1 2>;
   事先在上面已经找到了key引脚对应的外部中断是XEINT9，回到芯片手册中找SPI类型的外部中断9，
   发现外部中断9，对应了SPI port No的值为25.
   interrupts = <0 24 0>, <0 25 0>, <0 26 0>, <0 27 0>,
                <0 28 0>, <0 29 0>, <0 30 0>, <0 31 0>;
   所以知道外设key使用的UART_RING引脚使用<0 25 0>，这部分对应下标为1(所有内容从0开始标号的)

按键驱动的设备树添加内容:
591        key{
592            compatible = "farsight,key";
593            interrupt-parent = <&gpx1>;
594            interrupts = <1 2>,<2 2>;
595        };


struct resource *platform_get_resource(struct platform_device *dev,unsigned int type, unsigned int num)
功能:获得xxx[]结构体数组元素的首地址
已知:platform_device.c中有一个:
	 struct resource xxx[] = {
		{
			.start =
			.end =
			.flags =
		},
		{
			.start =
			.end =
			.flags =
		},
		{},
	 };
参数1:platform_device设备信息结构体首地址，在probe函数中可以直接使用
参数2:IORESOURCE_MEM   IORESOURCE_IRQ
参数3:xxx[]数组的下标


```
