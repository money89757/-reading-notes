# �ж� #
## �жϷ��� ##

\| \|
----|------
�ڲ��ж� | �ⲿ�ж�  
�����ж� | �������ж�
�������ж� | ���������ж�

* �ص���ĵ����ⲿ�ж�
����-> �жϿ����������νṹ������-> CPU

* HW interrupt ID Ӳ���жϺ�
irq number  
�ⲿ�жϣ� SPI��PPI��SGI

#### ����ϵͳ�ں��Ƿ�ʵ�����жϻ���
1. ��ʼ���жϿ�������Ӧ�Ĵ�����������Ӧ��һ�ѽṹ�壬irq_desc  irq_data  irq_chip irqaction  
2. ����request_irq�ӿ��������ж�ע��
```
����1:Ӳ���жϺ�  
����2:�жϴ�����  
����3:IRQF_DISABLEDʹ�ÿ����жϣ��������������жϡ�  
    IRQF_TRIGGER_FALLING �½��ش���  
����4:����   ���жϺź����ư󶨣�������/proc/interrupts  
����5:���ʹ�õ��ǿ��жϴ���NULL��ʹ��SPIʱ��Ҫ����
```
3. ����������ж�ʱ�����л����жϴ�������ִ�С�
���д��������������д�����������豸����  

```c

mykey{
	compatible = "";
	interrupt-parent = <&gpx1>;
	interrupts = <1 2>,<2 2>;
};

```
����ͨ�����Ǿ��л�е���Եİ�����  
������������:  
�����ʽ:ʹ�ö�ʱ����  
Ӳ����ʽ:����һ�����ݡ�  

### �����ʽ��������ʱ�� ###

```c
struct timer_list
{
21     void (* function)(unsigned long);                                        
22     unsigned long data;
}

```

1. ��ʼ����ʱ�� init_timer(struct timer_list * );
2. ��Ӷ�ʱ�����ں� add_timer();  
3. ���ö�ʱ������ʱʱ�� mod_timer();

```c
HZ��һ����λ   1HZ = 1��
HZ / 100 = 10ms  
jiffies + 3 * 10ms ���ö�ʱʱ��Ϊ��ʱ30����

```
## �ж��ϡ��°벿
ִ���ж�ʱ��֤ʱ������̣�����ԸΥ�ܶ�ʱ�����ǲ����жϴ�����ʱ������Ӻܶ๦�ܴ�������ж�ִ��ʱ�������  
����Ҫ�����ж�ʱ���������������ʹ���ϰ벿���°벿��
1. ʲô���ж��ϰ벿���°벿
���жϵĹ��ֳܷ������������ֱ�ִ�С�  
�ϰ벿:��������֮ǰʹ�ù����жϴ�����  
�°벿:ִ��һЩ�ǽ�������  

2. ʲô�ǽ�������?  
�ܽ�:1)���һ�������ʱ��ǳ����з����ϰ벿����  
	 2)��Ӳ���Ĳ��������ϰ벿  
	 3)���Ҫ��ִ��ʱ���ܱ������жϴ�Ϸ����ϰ벿  

3. ʲô���°벿��  
linux�ں�ʵ�ֺܶ�ܶ���°벿���ƣ����Ǵ�2.6�ں�֮����Ҫ�ľ����֡������жϡ�tasklet����������  
�������ִ��ֻ��Ҫ�˽����жϣ�ѧ��ʹ��tasklet���������С�  

4. �°벿ʲôʱ��ִ�У�  
ͨ����������ж��ϰ벿ִ����ɺ����°벿���ơ�  


#### ���ж� #####

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
�����ں�ռ�õ����ж�����жϺ�(Ҳ�������ж�����)

```

�����������ʹ�����ж���ʵ���°벿��  
ʹ�÷�����  
```c
1��void open_softirq(int nr, void (*action)(struct softirq_action *));ע�����ж�
2��void raise_softirq(unsigned int nr);�������ж�
��������������ں˲�û�������ű�Ҫ��ʹ�ñ��룬����kernel/softirq.c�ļ�
��open_softirq������raise_softirq����������������EXPORT_SYMBOL(������);
���±����ں�:make uImage

tasklet(С���񣬻������ж�ʵ�ֵ�)ʹ�÷���:
425 struct tasklet_struct                                                       
426 {
427     struct tasklet_struct *next;
428     unsigned long state;
429     atomic_t count;
430     void (*func)(unsigned long);//tasklet�°벿����������func�����ڲ���ɵ�
431     unsigned long data;
432 };

tasklet_init();��ʼ��tasklet
tasklet_schedule();����tasklet


```

#### ��������ʹ�÷��� ####
```c

100 struct work_struct {
101     atomic_long_t data;
102     struct list_head entry;
103     work_func_t func; //���������е��°벿��������
}
19 typedef void (* work_func_t)(struct work_struct* work);

��ʼ����������:INIT_WORK();
���ȹ�������:schedule_work();

tasklet�͹������е�����:
tasklet�����ж������ġ������������ڽ��������ġ�
tasklet�������ж��°벿��������ʱ�ģ��������п�����˯����ʱ����һ��Ľӿڣ��������һ���������ϵͳ������ȡ�

```

### ADC������д ###
```
adcӲ��ԭ��:ģ������ת����

ʹ��ADCʱ��Ҫ����:����ת�����ȡ�����ʱ��Ƶ�ʡ�ѡ��ͨ��
						    ADCCON             ADCMUX      

ADCDAT ��ȡת����ĵ�ѹֵ
		    1.8
	������������������������������������  * ADCDAT  ��ȡ��ʵ�ʵĵ�ѹֵ��ֵ��uvΪ��λ
		4096 * 1000

ADC���ݵĻ�ȡ��ʽ:��ѯ���ж�

��������ʹ���жϵķ�ʽ��ȡ���ݡ�д����ǰ��ʵ��adc�������豸����

������оƬ�ֲ�ھ��·����ⲿ�ж����ӵ�GIC���ڲ����ӵ�combiner��������
�������ֲ��ʮ���ҵ� INTG10  [3]   ��������ADC

vi Documentation/devicetree/bindings/arm/samsung/interrupt-combiner.txt
23 - #interrupt-cells: should be <2>. The meaning of the cells are
 24     * First Cell: Combiner Group Number.   ��ı��
 25     * Second Cell: Interrupt number within the group.   �������ӱ��  

#interrupt-cells �����˵�ǰ�ڵ���ӽڵ��interrupts������2��ֵ��


combiner�ڵ�λ��:arch/arm/boot/dts/exynos4.dtsi
�����Լ���adc�豸��:
fs4412-adc{
	compatible = "fs4412,adc";

	reg = <0x126C0000 0x20>;
	interrupt-parent = <&combiner>;
	interrupts = <10 3>;
};

data = readl(�Ĵ�����ַ); �ӼĴ����ж�ȡ����
writel(���ݣ��Ĵ�����ַ); ������д�뵽�Ĵ�����

```

### I2C ����Э�� ###
```c
I2C����Э��:
I2C�����Ƿ����ֹ�˾�������������ߵ��豸��

I2C���ߡ��������ߺ�ʱ����

I2C����ͨ��ʱ:��ʼ�źš������źš�Ӧ���ź�
��ʼ�ź�:��ʱ����Ϊ�ߵ�ƽ���������ɸߵ�������ʱ��������ʼ�ź�
�����ź�:��ʱ����Ϊ�ߵ�ƽ���������ɵ͵��ߵ�ƽ������������ź�

ÿ�δ�������һ����8λ���ڵھ�λһ����һ��Ӧ��λ��
Ӧ���ź�:ʱ���߱�֤�ߵ�ƽ�����ڼ������߱����͡�
		 �ڵھŸ�ʱ����������������յ�0˵���ӻ��������������ݡ�


I2C���ݵĴ����ʽ:
1������һֱ���ӻ���������
	����֡�ĸ�ʽ:��ʼ�źš���8λ���ݡ���Ӧ��(�ӻ�������)����8λ����(���ǵ�ַ�Ͷ�дλ)�����ӻ�������Ӧ�𡪡������ź�
							 ||
						ǰ��7λ�Ǵӻ���ַ
						�ڰ�λ�����ݷ���
	����8λ������:11010000 ǰ���7λ����ӻ���ַ�������0�����������ӻ���������
                  11010001 ǰ���7λ����ӻ���ַ�������1�����������մӻ����� 	
2������һֱ���մӻ�����
    ����֡�ĸ�ʽ:��ʼ�źš���8λ����(��7λ��ַ+1λ��1)�����ӻ�������Ӧ�𡪡��������յ�8λ���ݡ����������ӻ�Ӧ�𡪡������ź�

3�������������ݺ�������մӻ�����
    ����֡�ĸ�ʽ:��ʼ�źš���8λ����(7λ�ӻ���ַ+1λ��0)�����ӻ�������Ӧ�𡪡�����8λ���ݡ����ӻ�������Ӧ�𡪡�����7λ�ӻ���ַ+1λ��1�����ӻ�������Ӧ�𡪡��������յ�8λ���ݡ����������ӻ�Ӧ�𡪡������ź�   



```
