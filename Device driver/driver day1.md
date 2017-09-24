# driver day1

### 驱动概念
应用程序提供一系列的函数进而操作硬件的一种内核程序。

### 模块概念
可以在内核中随时添加或者删除的一段代码。模块可以提高效率。  
模块与应用程序比较：  

 | 模块 | 应用程序
----|------|----
执行空间 | 内核空间  | 用户空间
入口函数|加载函数|main
调用接口 | 内核函数  | 库函数或系统调用
空间释放 | 手动释放  | 自动释放

只有使用模块时才会占用内存空间，不使用时可以从内存空间释放出来。

### 模块文件的特点
```
模块文件名称：xxx.ko
模块名称：xxx

加载模块：insmod xxx.ko
卸载模块：rmmod xxx

显示系统加载过的模块：lsmod
如何显示内核的打印信息到终端：dmesg
清除内核打印信息：sudo dmesg -c
```

### 模块的三要素
加载函数、卸载函数、模块声明。
#### 模块加载函数
1. 默认加载函数`init_module()`
2. 自定义加载函数`module_init(模块入口)`
在Linux3.14 内核中显示为：

```
297 #define module_init(initfn)                 \                                                                             
298     static inline initcall_t __inittest(void)       \
299     { return initfn; }                  \
300     int init_module(void) __attribute__((alias(#initfn))); initfn是init_module的别名

135 typedef int (*initcall_t)(void); 《==》 typedef int(*)(void) initcall_t
initfn的类型是函数指针类型

module_init()中的参数其实是一个函数名称，这个函数返回值是int,形参是void

vim -t module_init 选择4
266 #define module_init(x)  __initcall(x);

212 #define __initcall(fn) device_initcall(fn)

207 #define device_initcall(fn)     __define_initcall(fn, 6)

176 #define __define_initcall(fn, id) \                                                                                       
177     static initcall_t __initcall_##fn##id __used \
178     __attribute__((__section__(".initcall" #id ".init"))) = fn

static initcall_t __initcall_##fn##id __used  ==》 static initcall_t __initcall_6id = fn
定义了__initcall_6id变量，这个变量类型是initcall_t
fn是我们模块中自定义的一个名称

__attribute__((__section__(".initcall" #id ".init"))) 说明__initcall_6id是在 .initcall6.init分段中的。

进入arch/arm/kernel/vmlinux.lds文件，寻找.initcall6.init分段
 __initcall6_start = .; *(.initcall6.init)
 发现__initcall6_start是所有.initcall6.init分段的起始地址
```

#### 模块卸载函数
1. 默认的卸载函数`cleanup_module()`;
2. 自定义卸载函数`module_exit(真正的自定义卸载函数明)`；

模块声明:MODULE_LICENSE("GPL");说明模块程序符合某种协议  
GPL:规定使用者可以免费开源获取源码，也可以修改源码进行二次开发，二次开发的源码必须要符合GPL协议(二次开发的内容也要开源)   
BSD:规定使用者可以免费开源获取源码，也可以二次开发，但是BSD协议鼓励个人的研发成果，二次开发的内容可以不使用BSD协议(这部分代码可以商业化)  

# 模块编译
hello.c
1. module_init();
2. module_exit();
3. 调用MODULE_LICENSE();这个宏函数在驱动代码中的任意全局位置出现。

## 模块编译的方式
### 1.内部编译
1. 拷贝`hello.c`到内核`drivers/char`中
2. 打开`driver/char/Kconfig`,添加  
```
    config FIRST_DRV  
      tristate "our first driver"
```  
3. 打开driver/char/Makefile,最后一行添加：
```
    obj-$(CONFIG_FIRST_DRV) += hello.o
```
4. `make menuconfig` 找到选项 `our first driver`并且选项为M
5. 顶层目录下执行`make modules` 会在`drivers/char` 目录下生成一个叫做`hello.ko`的模块文件
6. `cp driver/char/hello.ko /rootfs`
7. 开启开发板，然后在开发板上执行`insmod hello.ko`,最后去查看是否有打印信息
8. 卸载模块`rmmod hello` 提示缺少一些文件件

### 2.外部编译
1. 自己写一个Makefile来引用内核顶层的Makefile
2. 顶层Makefile需要知道编译外部的具体那个.o文件
```
Ubuntu的/lib/modules/$(shell uname -r)/build索引了Ubuntu的内核的顶层目录，
进入Ubuntu的顶层目录下Makefile 1181行有个提示:make M=dir modules  其中dir是用户自己的驱动所在路径
根据上面的提示，在自己的Makefile中写出make -C $(KERNEL_DIR) M=$(PWD) modules

```
3. 因为我们执行了make -C $(KERNEL_DIR) M=$(PWD) modules,所以我们需要在顶层目录Makefile中寻找modules目标
```
1206 modules: $(module-dirs)
1207     @$(kecho) '  Building modules, stage 2.';
1208     $(Q)$(MAKE) -f $(srctree)/scripts/Makefile.modpost  后续需要执行Makefile.modpost
```
4. 进入Makefile.modpost文件
```
53 include $(if $(wildcard $(KBUILD_EXTMOD)/Kbuild), \
54              $(KBUILD_EXTMOD)/Kbuild, $(KBUILD_EXTMOD)/Makefile) 在编译模块过程的某个时刻会引用用户自己的Makefile中
 回来的目的就是为了获取.o文件的。
```
5. 如何实现这两个分支：在内核Makefile中有一个KERNELRELEASE变量：
```
384 KERNELRELEASE = $(shell cat include/config/kernel.release 2> /dev/null)
有以上几个过程我们分析出自己的Makefile文件必须要有个判断语句:
ifeq ($(KERNELRELEASE),)
伪目标:
 make -C $(KERNEL_DIR) M=$(PWD) modules

else
 obj-m += xxx.o
endif
```
#### 模块传参
`module_param(参数名称，参数类型，权限);`  
权限如果是非0，在/sys/module/模块名称/parameters下会创建一些以参数名称命名的文件  
权限为0不会在/sys/module/模块名称/parameters下创建文件。  

` #define module_param_string(name, string, len, perm) `   
参数1:执行insmod命令时一个自定义名称  
参数2:是一个字符数组名，这个数组接收并且存放外部传递的字符串  
参数3:字符数组大小  
参数4:权限  

`module_param_array(name, type, nump, perm)`
参数1:insmod时参数名称，但也是某个数组名  
参数2:类型  
参数3:接收真正传递数据个数的变量地址。  
参数4:权限  
