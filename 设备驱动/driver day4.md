## 内核中cmd真正的使用方式
cmd本身是32位无符号整数。
 ```
 8 				 8 			    2 		    	    14
设备文件类型    命令的序号    命令的操作方向     传递的数据类型的类型大小

 ```

内核中:Documentation/ioctl/ioctl-number.txt 这个文件中  
会描述哪些类型可用哪些类型的序号可用
命令操作方向:无数据传输、读数据、写数据、读写数据
```c
_IO(类型，序号)  
_IOR(类型，序号，代表了类型大小) 一定要注意:使用时只需要传递类型  
_IOW(类型，序号，代表了类型大小)  
_IOWR(类型，序号，代表了类型大小)  
```
### ioremap();将物理地址转化为虚拟地址

platform总线:不是物理总线，内核维护的一种软件机制。  
platform总线分别挂载了设备信息和驱动信息。总线还会提供驱动和设备的匹配机制。  
因为platform总线的出现，可以实现设备和驱动的分离。  
e.g
```c
int add(int a,int b)
{
	return a + b;
}

main()
{
	add(10,20);
}
驱动和设备结合,逻辑简单、但是移植性很差
————————————————————————————————————————————————————
struct dev {
	int id;
	int a;
	int b;
};

struct dev mydev = {
	.id = 1,
	.a = 10,
	.b = 20,
};
上面模拟的是设备信息
——————————————————————————————————————————-
struct drv{
	int id;
	int (*add_p)(struct dev * );
};

struct drv mydrv = {
	.id = 1,
	.add_p = add,
};


int add(struct dev *info)
{
	return info->a + info->b;
}
模拟驱动代码
————————————————————————————————————————————————————
int bus()
{
	if(mydrv.id == mydev.id)
		return mydrv.add(&dev);
}
模拟的总线机制

```

#### 真正操作platform总线时：
1. 实现总线
2. 实现外设驱动
3. 实现外设信息

#### 如何实现platform设备文件、platform驱动文件
```C
struct platform_device {                                                                                                  
23     const char  * name; //存放任意字符串，用来和驱动匹配的。
24     int     id; //给设备编号的，如果只有一个赋值为-1。
26     struct device   dev;                                                                                                  
27     u32     num_resources; //资源个数
28     struct resource * resource; //指向的结构体专门存放外设信息的。
}

struct device
{
 783     void    (* release)(struct device * dev);//释放设备资源
}

18 struct resource {                                                                                          
19     resource_size_t start; //硬件资源的起始地址
20     resource_size_t end;  //硬件资源的结束地址
22     unsigned long flags;  //代表了硬件资源的类型，比如:IORESOURCE_MEM，IORESOURCE_IRQ
}

platform_device_register();//将设备信息添加到总线维护的设备链表中。
platform_device_unregister();

```

### 如何编写platform设备文件
```c
1、模块声明
	struct platform_device_id myid_table[] = {
		{"123"},
		{"456"},
		{},
	};
	4、定义并且初始化platform_driver结构体
		{
			.probe = xxx_probe,
			.remove = xxx_remove,
			.driver = {
				.name = "abc",
			}
			.id_table = myid_table,
		}

2、加载模块
	5、platform_driver_register();
3、卸载模块
	6、platform_driver_unregister();
```
