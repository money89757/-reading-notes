# 动态创建设备文件
在linux内核版本2.6之后出现了一种文件系统sysfs,这个文件系统挂载到了/sys目录下，在sys目录下有很多子目录，  
每个子目录中都会出现一个叫做uevent的文件，这个文件存放了主次设备号的信息以及设备文件名信息。sysfs文件系统  
会将内核中新产生的设备号信息以及设备文件信息存放到uevent文件中。  

在linux操作系统，有一类应用程序，比如udev、mdev可以访问/sys下的uevent文件提取出uevent文件中的主、次设备号、设备文件名，  
进而在/dev目录下创建出设备文件。  

## 具体函数实现
#### `class_create(owner, name)` 创建设备类
* 参数一：THIS_MODULE
* 参数二：类的名称    这个类的名称会出现在/sys/class/类的名称/
返回值类型:struct class *

#### `struct device *device_create(struct class *class, struct device *parent,dev_t devt, void *drvdata, const char *fmt, ...)`
功能:自动的在/dev下创建相应的设备文件
* 参数1:class_create函数的返回值
* 参数2:父类设备，几乎传递NULL
* 参数3:某类目录下具体的设备的设备号
* 参数4:给子设备传递的参数，一般使用NULL
* 参数5：本身是一个软链接的名称，这个名称和最终在/dev目录下创建的设备文件名一致
* 参数6:这里的...不代表不定参，要么使用要么就不使用。使用时和参数5有关。


#### class_destroy() 删除设备类
#### device_destroy() 删除设备文件
---
```c
file_operations
{
	int (* open)(struct inode * ,struct file * );
}
```
struct inode 结构体只要文件被创建，相应的会出现inode结构体。比如字符设备文件被创建成功，  
在内核中inode结构体描述文件的静态信息。  
struct file 描述文件的动态信息。文件每打开一次就会创建一个struct file结构体  
对于一个指定文件来说:inode结构体是唯一的，file结构体和文件打开的次数相关.  

## 内核中open函数的作用
1. 实现计数——原子操作。
2. 申请资源——内核锁机制。
3. 设备文件的主、次设备号。
```C
int demo_open(struct inode *inode,struct file *filp)
{
	major = MAJOR(inode->i_rdev);
	minor = MINOR(inode->i_rdev);
}
```
> 应用层的open调用了底层驱动的open;  
> 应用层的close调用底层的release;  
---
`int (*release) (struct inode *, struct file *);`
 功能和close一致。

---
打开文件的基本目的:读文件、写文件(软件层次上提到了读和写都是站在用户空间的角度)
IO讲的读写:
读:数据从内核空间流向用户空间
写:反向

驱动中所谓的读写:
读:数据从内核空间流向用户空间
写:相反

---
#### 应用层:read
```c
int       read(      int fd,   char buf,   size_t size   );  
						||           ||         ||
ssize_t (*read)(struct file *,char __user *,size_t ,    loff_t *)  
参数1:通过应用层的文件描述符索引的。  
参数2:接收了用户程序中读数据的空间首地址  
参数3:接收了应用程序需要读取的最大字节数  
参数4:代表了读文件时，文件指针到文件头的偏移量.  

		   想实现数据拷贝需要使用copy_to_user();  

static inline unsigned long copy_to_user(void __user *to, const void *from, unsigned long n)  
功能:将驱动的数据拷贝给用户程序  
参数1:应用程序使用的空间地址  
参数2:内核空间指定的地址  
参数3:实际需要拷贝的字节数  
返回值:成功返回0   这个函数注意,使用时一定要判断错误。  
```
---

#### 应用层:write
```C
int write(          int fd,   const char *buf,      size_t size);  
					   ||	              ||          ||
ssize_t (*write)(struct file *,  const char __user *,size_t ,    loff_t *)  
write函数的目的是:将用户空间的数据拿到内核空间中。  
使用方法是调用copy_from_user();  

static inline unsigned long copy_from_user(void *to, const void __user *from, unsigned long n)  
功能:考本用户空间数据到内核空间  
参数1:内核空间某个首地址  
参数2:用户空间某个首地址  
参数3:实际拷贝数据大小  
返回值:成功返回0  
```
---

```
832 #define container_of(ptr, type, member) ({          \
833     const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
834     (type *)( (char *)__mptr - offsetof(type,member) );})

const typeof( ((type *)0)->member ) *__mptr = (ptr);

(type *)0  将0强转为一个地址，类型为type *
((type *)0)->member 说明强转后的这个类型对应的变量内部有一个成员叫做member
总结出type是一个结构体类型或者共用体类型。

typeof( ((type *)0)->member ) 获取member的类型
typeof的作用:获取某个变量的类型
const typeof( ((type *)0)->member ) *__mptr 定义了一个指针变量
以上几步可以总结出ptr和member的基本的类型(不考虑指针)是一样的。

19 #define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
((TYPE *)0  0编号地址
((TYPE *)0)->MEMBER 以0编号地址为基础引用MEMBER成员
&((TYPE *)0)->MEMBER 获取MEMBER成员的地址
(size_t) &((TYPE *)0)->MEMBER  将MEMBER成员的地址转为一个整数。

container_of总体总结:根据一个结构体中的已存在成员类型获取这个成员所在结构体的首地址。

```
