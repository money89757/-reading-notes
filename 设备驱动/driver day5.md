### struct resource
```c
18 struct resource {
 19     resource_size_t start;//可能是起始寄存器地址
 20     resource_size_t end;//结束寄存器地址
 21     const char * name;
 22     unsigned long flags;//如果使用的是寄存器地址，这个成员使用IORESOURCE_MEM
 23     struct resource * parent, * sibling, * child;
 24 };

```
上面的结构体专门描述某个设备硬件信息。

/sys/devices/platform/ 如果设备被正确注册，这个目录下会生成一个以.name = "xxx"，以这个xxx命名的一个文件夹

### 设备树
```C
在u-boot中:setenv bootcmd tftp 41000000 uImage\;tftp 42000000 exynos4412-fs4412.dtb\;bootm 41000000 - 42000000
重点在bootm 42000000代表了设备树二进制文件的地址。
加载内核:41000000内核的uImage文件临时存放地址。
内核真正地址:40008000
参数的物理地址:40000100 其中设备树信息包含于参数。

```
内核启动时有一个判断条件——判断启动参数是否使用了设备树  
```c
进入start_kernel()
		||
		\/
	setup_arch();
		||
		\/
	setup_machine_fdt(设备树文件的虚拟地址);
	unflatten_device_tree();
内核解析设备树的源码,解析后最终会将设备树中每个节点解析为一个叫做device_node的结构体，这个device_node
最终会被添加到某些内核维护的单向链表中，比如platform总线;platform总线可以遍历驱动信息。
在内核中有一堆的函数接口以of开头，其中一个函数是:of_find_compatible_node().

```
# 设备树基本概念
文件名：dts.dtsi.dtb  

.dts _dtc__> .dtb 文件  

设备树文件没有任何的逻辑，只有描述性的语句。  

设备树文件包含了节点和属性：  
*每个设备树文件有且仅有一个根节点，用/表示*  
其他的所用节点都包含于根节点。
```c
标号:节点名@地址{
	属性 = 值;
};

属性 = &标号

属性compatible:
a{
	compatible = "xxx1,yyy1","xxx1,yyy2"; 这里的每个字符串都会和驱动中的某个结构体成员匹配。
	reg = <基地址 偏移量>
}

```

设备树中的compatible属性最终会被转化为字符串名称和字符串内容，分别存放在某个链表中。  
驱动为了和设备信息匹配也应该有相同的字符串名称以及相同的内容。
```c
struct platform_driver
     ||
     \/
struct device_driver driver;
     ||
     \/
const struct of_device_id   *of_match_table
     ||
     \/
    char    compatible[128]; 在驱动中需要自己添加的内容，我们只保证这里的内容和设备树中compatible属性的内容一致
```


# 实现驱动和设备树匹配
## 驱动中：
```c
struct of_device_id led_dt_table[] = {
	{"xxx,yyy"},
	{},
};

struct platform_driver pdrv = {
	.driver = {
		.of_match_table = led_dt_table,
	},
};

```
编译驱动的外部模块:  
cp xxx.ko /rootfs  
启动文件后再开发板执行insmod xxx.ko

## 设备树中：
```C
/{
	节点名称{
		compatible = "xxx,yyy";
		reg = <寄存器地址 偏移量>  3个地址 3个偏移量
	};
};

```
到内核顶层目录下执行make dtbs  
```C
cp arch/arm/boot/dts/exynos4412-fs4412.dtb  /tftpboot

```

#### 并发：多个任务宏观并行，微观串行（也可以并行，涉及到多核）
* CPU和CPUz之间：每个核都可以运行一个进程
* 单核CPU中进程和进程之间：宏观并发(基于操作系统是可抢占的)
* 单核CPU中进程和中断
* 单核CPU中中断和中断

#### 并发：提高操作系统的执行效率。
事物会有两面性，因为并发就有可能出现竞态。  
多个任务同时操作同一空间(可以是一个虚拟空间的地址，可也以是一个设备)。  
为了避免静态，我们会使用同步互斥。

##内核中的同步互斥
1. 中断屏蔽
2. 原子操作
3. 互斥锁
4. 自旋锁
5. 信号量  
