```
用户空间3G
			   库函数的调用(操作流缓存区中的数据)
				 ||	
			   流缓存区
—————————————————||——————————————————————————
内核空间1G     系统调用  
				 ||
			  虚拟文件系统	
				 ||	
			   驱动程序——>内核缓存区 	   
—————————————————————————————————————————/\———
硬件           键盘                      || 读

```

```
IO：标准IO/缓存IO/高级IO  文件IO/非缓存IO/低级IO
		||                       ||
	  库函数(man 3)            系统调用(man 2)
```

#### 读写的过程:
标准IO:读:数据->内核缓存区——>流缓存区——>库函数使用  
文件IO:读:数据->内核缓存区——>库函数使用  

#### API:应用程序编程接口(用户空间中的函数的声明)
#### 系统调用:主要代表的是内核空间中的函数源码(函数的定义)

`注意:系统调用不是内核函数`


### 流缓存区特点:
行缓存: 缓存区满才能读写，linux下默认大小为1K，和终端相关的读写是行缓存。遇见\n可以刷新缓存区，调用fflush函数刷新  
全缓存：缓存区满才能读写，默认大小4K，和文件读写相关的是全缓存。调用fflush函数刷新，fclose也可以刷新缓存区  
不带缓存:stderr   

```
FILE *fopen(const char *path,const char *mode);

r:只读，文件必须事先存在，打开后文件指针指向文件开头
r+:上面的基础多了写权限。覆盖写数据

w:只写，文件不存在创建文件，存在清空文件。
w+:上面的基础多了读权限。

a:只写，追加写。文件不存在创建，存在存在从文件末尾追加
a+:读写权限。文件不存在创建。
   文件存在:先读后写，从头开始读，不管读到哪里，一定从末尾写数据
		    先写后读，从末尾写数据，从末尾读数据
```
---

```
int fgetc(FILE *)
从文件中读取一个字符，成功返回字符的ascii值，失败或者到达尾部返回EOF
```

```

feof(FILE *) 返回值为非0到达尾部，返回值为0没有到达尾部

while(1)
{
	ch = fgetc(fp);
	
	if(ch == EOF && feof(fp) == 0)
		出错
	if(ch == EOF && feof(fp) != 0)
		到达尾部
}

```

```
char *fgets(char *,int size,FILE *)
从文件中最多读取一行数据，size满足足够大。
具体来说读取size-1个字节的数据
```

```
size_t fread(void *ptr,size_t size,size_t nmemb,FILE *);
从文件中读取 size乘以nmemb 个字节数据
size代表了每个对象的大小
nmemb代表最多读取的对象个数


返回值:等于nmemb读取成功，小于nmemb说明可能到达了文件尾部也可能中途出错

```

```
int open(const char *pathname,int flags);

参数1:代表要打开的文件名，应用层主要打开普通文件，底层主要打开设备文件
参数2:O_RDONLY O_WRONLY O_RDWR  必须出现其中的一个，只能出现其中的一个
	  O_CREAT创建文件  O_TRUNC清空文件  O_APPEND追加文件
	  O_EXCL文件已存在报错
返回值:成功返回一个文件描述符，失败返回-1 


fopen("1.txt","r"); <==> open("1.txt",O_RDONLY);
fopen("1.txt","r+");<==> open("1.txt",O_RDWR);
						 open("1.txt",O_WRONLY);
						 
fopen("1.txt","w"); <==> open("1.txt",O_WRONLY | O_CREAT | O_TRUNC,0664);  open函数只要使用了O_CREAT必须要用到第三个参数
fopen("1.txt","w+");<==> open("1.txt",O_RDWR | O_CREAT ｜O_TRUNC,0664);

fopen("1.txt","a");<==> open("1.txt",O_WRONLY | O_CREAT | O_APPEND,0664);
fopen("1.txt","a+");<==> open("1.txt",O_RDWR | O_CREAT | O_APPEND,0664);

stdin <==> 0  stdout <==> 1  stderr <==> 2
```

```
ssize_t read(int fd,void *buf,size_t size);
功能:从文件中读取数据存放到buf空间里
参数1:文件描述符
参数2:存放了读取到的数据
参数3:存放数据的空间大小
返回值:成功返回读取到的字节个数，0代表到达文件尾部，-1出错

```

```

ssize_t write(int fd,const void *buf,size_t size);
功能:将buf指向的空间中的数据写入到文件中
参数3:代表了实际写入的数据字节数

```

```

off_t lseek(int fd,off_t offset,int whence);
功能:实现文件指针的偏移或者定位
参数2:偏移量
参数3:从哪个位置开始偏移(SEEK_SET从头  SEEK_END从尾部  SEEK_CUR从当前)

```


### 库特性:
1. 什么是库
函数源码的二进制(某种特殊格式的二进制，不是可执行文件)文件。  

2. 库的分类  
静态库，动态库(共享库)  

3.库的名称  
静态库:以lib开头，以.a结尾  
动态库:以lib开头，以.so结尾  

```

例子:libabc.a 这个整体叫做静态库，而其中的abc叫做静态库名称
	 lib123.so 整体叫做动态库，其中123叫做动态库名称
```


4.库的特点  
```
a、静态库:使用静态库时，库的相关二进制信息会被直接加载到可执行程序中。
		  优点:如果可执行程序再使用过程中静态库被删除，不会影响可执行程序的使用。
		  缺点:如果可执行程序中对于同一个函数调用了多次，同一个静态库会被重复加载多次。会造成可执行文件比较大浪费内存空间
b、动态库:动态库的信息不会在编译时加载到可执行文件中，当调用到具体的函数时才会被加载到可执行文件中运行，
		  如果对于同一个函数调用多次，动态库只会加载一次。

```

5. 库的创建过程
```

静态库:    1、准备好某些函数的源代码
	   2、编译函数源代码生成.o文件
	   3、ar -cr lib库名称.a  需要的.o文件     作用是产生静态库
使用静态库:
第一种:如果静态库没有存放到系统路径下：gcc main.c -L 静态库路径 -l 库的名称 
第二种:如果静态库存放在系统路径下(/lib  /usr/lib /usr/local/lib ...)
		gcc main.c -l 库的名字
		
动态库:1、准备好函数的源码
	   2、编译生成.o文件 gcc -c -fpic .c文件 -o .o文件
		  fpic:位置无关码，源码中的函数以及一些变量在加载到内存之前不会有指定好的地址。
	   3、gcc -shared -o lib库名称.so  .文件  	
```
```

使用动态库:
如果动态库没有存放到系统路径下:gcc main.c -L 动态库路径 -l 动态库文件名称
但是执行时发现出错:./a.out: error while loading shared libraries: libmydlladd.so: cannot open shared object file: No such file or directory

如何解决上面的错误:
1、拷贝动态库到系统路径下:gcc main.c -l 动态库名称  
2、如果动态库没有存放到系统路径下：在/etc/bash.bashrc文件最后一行添加export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:动态库绝对路径 
	如果需要使bash.bashrc配置文件生效，需要执行source /etc/bash.bashrc来重启配置文件
	gcc main.c -L 动态库路径 -l 动态库文件名称
	./a.out 可以正常执行
```













