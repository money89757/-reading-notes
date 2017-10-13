# 中断 #
## 中断分类 ##

\| \|
----|------
内部中断 | 外部中断  
向量中断 | 非向量中断
可屏蔽中断 | 不可屏蔽中断

* 重点关心的是外部中断
外设-> 中断控制器（树形结构级联）-> CPU

* HW interrupt ID 硬件中断号
irq number  
外部中断： SPI、PPI、SGI

#### 操作系统内核是否实现了中断机制
1. 初始化中断控制器相应寄存器、产生相应的一堆结构体，irq_desc  irq_data  irq_chip irqaction  
2. 调用request_irq接口来进行中断注册
```
参数1:硬件中断号  
参数2:中断处理函数  
参数3:IRQF_DISABLED使用快速中断，屏蔽其它所有中断。  
    IRQF_TRIGGER_FALLING 下降沿触发  
参数4:名称   将中断号和名称绑定，出现在/proc/interrupts  
参数5:如果使用的是快中断传递NULL，使用SPI时需要传参
```
3. 当外设产生中断时，会切换到中断处理函数中执行。
如果写按键驱动。先填写按键驱动的设备树。  

```c

mykey{
	compatible = "";
	interrupt-parent = <&gpx1>;
	interrupts = <1 2>,<2 2>;
};

```
按键通常都是具有机械弹性的按键。  
按键消抖方法:  
软件方式:使用定时器。  
硬件方式:并联一个电容。  

### 软件方式：操作定时器 ###

```c
struct timer_list
{
21     void (* function)(unsigned long);                                        
22     unsigned long data;
}

```

1. 初始化定时器 init_timer(struct timer_list * );
2. 添加定时器到内核 add_timer();  
3. 设置定时器的延时时间 mod_timer();

```c
HZ是一个单位   1HZ = 1秒
HZ / 100 = 10ms  
jiffies + 3 * 10ms 设置定时时间为延时30毫秒

```
## 中断上、下半部
执行中断时保证时间进来短，事与愿违很多时候我们操作中断处理函数时都会添加很多功能代码造成中断执行时间过长。  
所以要避免中断时间过长。方法就是使用上半部和下半部。
1. 什么是中断上半部、下半部
将中断的功能分成两个函数来分别执行。  
上半部:就是我们之前使用过的中断处理函数  
下半部:执行一些非紧急任务  

2. 什么是紧急任务?  
总结:1)如果一个任务对时间非常敏感放在上半部运行  
	 2)对硬件的操作放在上半部  
	 3)如果要求执行时不能被其他中断打断放在上半部  

3. 什么是下半部？  
linux内核实现很多很多的下半部机制，但是从2.6内核之后主要的就三种——软中断、tasklet、工作队列  
上面三种大家只需要了解软中断，学会使用tasklet、工作队列。  

4. 下半部什么时候执行？  
通常情况下在中断上半部执行完成后唤醒下半部机制。  


#### 软中断 #####

```c
include/linux/interrupt.h

341 /* PLEASE, avoid to allocate new softirqs, if you need not _really_ high
342    frequency threaded job scheduling. For almost all the purposes
343    tasklets are more than enough.                  
345  */

347 enum
348 {
349     HI_SOFTIRQ=0,
350     TIMER_SOFTIRQ,
351     NET_TX_SOFTIRQ,
352     NET_RX_SOFTIRQ,
353     BLOCK_SOFTIRQ,
354     BLOCK_IOPOLL_SOFTIRQ,
355     TASKLET_SOFTIRQ,                                                        
356     SCHED_SOFTIRQ,
357     HRTIMER_SOFTIRQ,
358     RCU_SOFTIRQ,    /* Preferable RCU should always be the last softirq * /
359 	LXS_SOFTIRQ,
360     NR_SOFTIRQS
361 };
以上内核占用的软中断相关中断号(也叫做软中断类型)

```

如何在驱动中使用软中断来实现下半部：  
使用方法：  
```c
1、void open_softirq(int nr, void (*action)(struct softirq_action *));注册软中断
2、void raise_softirq(unsigned int nr);唤醒软中断
上面的两个函数内核并没导出符号表，要想使用必须，进入kernel/softirq.c文件
在open_softirq函数和raise_softirq函数定义的下面添加EXPORT_SYMBOL(函数名);
重新编译内核:make uImage

tasklet(小任务，基于软中断实现的)使用方法:
425 struct tasklet_struct                                                       
426 {
427     struct tasklet_struct *next;
428     unsigned long state;
429     atomic_t count;
430     void (*func)(unsigned long);//tasklet下半部操作就是在func函数内部完成的
431     unsigned long data;
432 };

tasklet_init();初始化tasklet
tasklet_schedule();调度tasklet


```

#### 工作队列使用方法 ####
```c

100 struct work_struct {
101     atomic_long_t data;
102     struct list_head entry;
103     work_func_t func; //工作队列中的下半部操作函数
}
19 typedef void (* work_func_t)(struct work_struct* work);

初始化工作队列:INIT_WORK();
调度工作队列:schedule_work();

tasklet和工作队列的区别:
tasklet用于中断上下文、工作队列用于进程上下文。
tasklet操作的中断下半部不能有延时的；工作队列可以有睡眠延时阻塞一类的接口，还像进程一样参与操作系统任务调度。

```

### ADC驱动编写 ###
```
adc硬件原理:模拟数字转换器

使用ADC时需要经历:设置转换精度、设置时钟频率、选择通道
						    ADCCON             ADCMUX      

ADCDAT 获取转换后的电压值
		    1.8
	——————————————————  * ADCDAT  获取的实际的电压值、值以uv为单位
		4096 * 1000

ADC数据的获取方式:轮询、中断

我们现在使用中断的方式获取数据。写驱动前，实现adc驱动的设备树。

在三星芯片手册第九章发现外部中断连接到GIC、内部连接到combiner控制器上
到三星手册第十章找到 INTG10  [3]   操作的是ADC

vi Documentation/devicetree/bindings/arm/samsung/interrupt-combiner.txt
23 - #interrupt-cells: should be <2>. The meaning of the cells are
 24     * First Cell: Combiner Group Number.   组的编号
 25     * Second Cell: Interrupt number within the group.   在组中子编号  

#interrupt-cells 代表了当前节点的子节点的interrupts属性有2个值。


combiner节点位置:arch/arm/boot/dts/exynos4.dtsi
我们自己的adc设备树:
fs4412-adc{
	compatible = "fs4412,adc";

	reg = <0x126C0000 0x20>;
	interrupt-parent = <&combiner>;
	interrupts = <10 3>;
};

data = readl(寄存器地址); 从寄存器中读取数据
writel(数据，寄存器地址); 将数据写入到寄存器中

```

### I2C 总线协议 ###
```c
I2C总线协议:
I2C总线是飞利浦公司生产具有两根线的设备。

I2C总线——数据线和时钟线

I2C总线通信时:起始信号、结束信号、应答信号
起始信号:当时钟线为高电平，数据线由高到低跳变时产生了起始信号
结束信号:当时钟线为高电平，数据线由低到高电平跳变产生结束信号

每次传输数据一定是8位，在第九位一定是一个应答位。
应答信号:时钟线保证高电平，这期间数据线被拉低。
		 在第九个时钟周期如果主机接收到0说明从机正常接收了数据。


I2C数据的传输格式:
1、主机一直给从机发送数据
	数据帧的格式:起始信号——8位数据——应答(从机给主机)——8位数据(不是地址和读写位)——从机给主机应答——结束信号
							 ||
						前面7位是从机地址
						第八位是数据方向
	假设8位数据是:11010000 前面的7位代表从机地址，后面的0代表主机给从机发送数据
                  11010001 前面的7位代表从机地址，后面的1代表主机接收从机数据 	
2、主机一直接收从机数据
    数据帧的格式:起始信号——8位数据(高7位地址+1位的1)——从机给主机应答——主机接收的8位数据——主机给从机应答——结束信号

3、主机发送数据后继续接收从机数据
    数据帧的格式:起始信号——8位数据(7位从机地址+1位的0)——从机给主机应答——发送8位数据——从机给主机应答——发送7位从机地址+1位的1——从机给主机应答——主机接收的8位数据——主机给从机应答——结束信号   



```
