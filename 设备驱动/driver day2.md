### 模块符号的导出
> 符号表 :函数名称或者变量名称  
有些通用的已经实现的函数或者变量在本模块中不需要冲洗实现，  所以可以引用实现过的符号

导出符号表的方法
```C
EXPORT_SYMBOL_GPL(符号名称);
EXPORT_SYMBOL(符号名称);
```

具体操作
```C
模块A：
static int value = 101;
static int add(int a,int b)
{
	return a + b;
}

编译模块A，并且加载模块A
cp Module.symvers文件到模块B

模块B：
extern int value;
extern int add(int,int);

编译模块B，加载模块B
```

## 驱动设备框架
* 按照字节的方式读写数据，一般情况是顺序操作。常见的字符设备(鼠标、键盘、LCD)

* 按照字节块的大小(512字节，或者是大于512字节并且是2的n次方)

* 网络设备:没有具体的框架

# 字符设备框架
```
用户空间
			应用程序
―――――――――――――――||―――――――――――――――――――――――――――――
内核空间	   ||
			   \/
			系统调用
			   ||
			   \/
			虚拟文件系统
			 ||        ||
			 \/        \/
		字符设备文件 块设备文件
			 ||        ||
			 \/        \/
		 匹配字符驱动 匹配块驱动
			 ||
			 \/
		  实现驱动
―――――――――――――||―――――――――――――――――――――――――――――――――――
			 \/
		    硬件		
```

### 如何创建设备文件
1. sudo mknod "设备文件名" c 500 0  
创建一个字符设备文件  
500是主设备号 0 是次设备号 这里设备文件名和设备号绑定在一起  
2. 内核提供一系列的函数来动态创建设备文件

#### 通过设备号匹配驱动
设备号本身就是一个无符号的32位整数，被分成两部分  
（前12位是主设备号、后20位是次设备号）
`主设备好代表了是一种设备，此设备好代表了同种设备中的具体设备`。
在驱动接口中设备号和驱动中的结构或者函数也需要绑定。  

## 如何产生设备号？
注意在操作系统中肯定注册过了一些设备号，  我们自己在注册设备号时不能和已存在的设备号相同。  
Ubuntu中:/proc/devices (提示的已使用的是主设备号)  
linux-3.14:Documentation/devices.txt     保证主设备好大于260就行

### 静态实现设备号
1. devno = 数组1<<20|数字2
2. devno = MKDEV(主设备号、次设备号);


### 动态实现设备号
内核中关键结构体、内核中调用的关键函数。  
vim -t cdev

```c
12 struct cdev {                                                                                                      
13     struct kobject kobj;
14     struct module  * owner; 使用时只会传THIS_MODULE，不需要纠结任何时候就这么用。代表当前模块
15     const struct file_operations * ops; 指向了file_operations结构体
16     struct list_head list;
17     dev_t dev;  存放了设备号。  暂时假设一下:这里能不能被赋值为设备号。
18     unsigned int count;
19 };

1525 struct file_operations {
   都是一些函数指针;――》都指向驱动中的相应操作函数。
}

```
### 动态实现设备号关键函数
1. `int register_chrdev_region(dev_t from, unsigned count, const char *name) `  
功能:将我们自己设置好的设备号添加到内核中。  
一旦添加成功会在/proc/devices文件中出现相关的设备文件以及主设备号信息
参数1:起始设备号    有可能一次性注册多个设备  
参数2:注册的设备个数  不同种设备  
参数3:设备文件名    
返回值:0成功，负数出错  
struct file_operations myops;  

2. `void cdev_init(struct cdev *cdev, const struct file_operations *fops)`  
功能:初始化cdev结构体    
就是给cdev结构体中的ops指针赋值  

3. `int cdev_add(struct cdev *p, dev_t dev, unsigned count)`  
功能:将设备号赋值给cdev结构体的dev成员，为了后续和设备文件的设备号匹配的。  
参数1:  
参数2:起始设备号  
参数3:连续的次设备号个数  
返回值:0正确,负数是错误  

4. `void unregister_chrdev_region(dev_t from, unsigned count)`  
功能:注销设备号   
参数1:需要注销的起始设备号  
参数2:需要注销的设备个数   

5. `void cdev_del(struct cdev * p)`   
功能:将cdev中的dev成员内容清掉，并且删除cdev结构体  


# 字符设备操作流程
```c
3、定义并且初始化file_operations结构体
	{
		.owner = THIS_MODULE,
		.open = xxx_open,
	};
模块的加载函数:
1、定义初始化设备号  MKDEV()；
2、注册设备号   register_chrdev_region();
4、初始化cdev结构体  cdev_init();
5、将cdev结构体添加到内核中  cdev_add();

模块的卸载函数:
6、注销设备号 unregister_chrdev_region();
7、删除cdev结构体 cdev_del();
```

# 字符设备框架优化
```C
1、int alloc_chrdev_region(dev_t *dev, unsigned baseminor, unsigned count,const char *name)
功能:动态注册设备号
参数1:设备号变量首地址
参数2:起始次设备号
参数3:次设备号个数
参数4:设备文件名称
返回值:成功返回0，失败返回错误码

2、动态给cdev结构体申请空间
static inline void *kzalloc(size_t size, gfp_t flags)
参数1:需要申请的空间大小
参数2:GFP_USER在用户层申请空间
	  GFP_KERNEL 在内核层申请空间

返回值:成功返回的是某段空间首地址

当一个函数返回值是一个地址时，如何判断出错误码？
if(IS_ERR(void * ))  IS_ERR的返回值如果为真说明有错误出现
{
	ret = PTR_ERR(void *) 获取错误码
	goto 标号;
}
```
