
# C语言与C预编译

首先三中形式的命令：宏定义，文件包含，条件编译命令。

## 1. 宏定义主要是：#define ，#undef

```C

#defien PI 3.1415926//宏定义

#defien Max(a,b) a>b?a:b//数的宏定义


```

FAILED用于检测数据的正确性
1. #define 应用
```C
#define 的应用

#define FAILED(Staus) ((Status) < 0)
#include <stdio.h>

void main()
{
	int d;
	printf("input number(n>0)\n");

	do
	{
		scanf("%d",&d);
	}
	while(FAILED(d))
}
```

`其中while(FAILED(d))在编译之前被无条件替换为while(d<0)。   
宏定义和调用在形式与函数比较相似，但是原理是不同。`

2. #undef 的应用
```c
#include "stdio.h"
void Test();
int main(int argc, char* argv[])
{
#define CONST_NAME1 "CONST_NAME1"
printf("%s/n",CONST_NAME1);
#undef CONST_NAME1
printf("%s/n",CONST_NAME1); /*错误，CONST_NAME1的定义已经取消*/
{
#define CONST_NAME2 "CONST_NAME2"
printf("%s/n",CONST_NAME2);
}
printf("%s/n",CONST_NAME2);
return 0;
}
void Test()
{
printf("%s/n",CONST_NAME2);
}

```
在系统的编译的时候，系统提示如下信息
```
error C2065:"CONST_NAME1" :undeclared indentifier

```
错误原因：在第二次应用符号常量`CONST_NAME1`时已被取消。

## 2.文件包含
```C
#include <文件名> ，这种属于标准方式，用于编译系统指定的文件。
#include “文件名”，这种属于用户方式，查找用户当前工作的文件夹中的文件，如果不存在则再按照标准方式查询。

```

## 3. 条件编译(常见的三中形式)

```C
①第一种形式：
#if defined(或者是ifdef)<标识符>
<程序段1>
[#else
<程序段2>]
#endif 

②第二种形式：
#if !defined(或者是ifndef)<标识符>
<程序段1>
[#else
<程序段2>]
#endif

③第三种形式常用与C++编译器中。
#ifdef …
[#elif … ]
[#elif …]
#else …
 #endif


```

## 4. 其他条件编译命令
1. error:
语法格式如下：  
error token-sequence
器主要的作用是在编译的时候输出编译错误信息token-sequence,  方面检查程序中出现的错误
```C

#include <stdio.h>

int main(int argc, char* argv[])
{

#define CONST_NAME1 "CONST_NAME1"
	printf("%s\n",CONST_NAME1);

#undef CONST_NAME1
#ifdef CONST_NAME1
#error No defined Constant Symbol CONST_NAME1
#endif
{
	#define CONST_NAME2 "CONST_NAME2"
		printf("%s\n",CONST_NAME2);
}
		printf("%s\n",CONST_NAME2);
		return 0;
}

```


编译的时候输出如编译信息
`fatal error C1189: #error : No defined Constant Symbol CONST_NAME1`

### 2. pragma
#### message
在编写程序的时候,我们经常要用到pragma 指令来设定编译器的状态或者是指示编译器完成一些特定的动作.  
一般格式：
`#pragma para`  

当编译器遇到这条指令时就在编译输出窗口中将消息文本打开， 我们自己有可能都会忘记有没有正确的设置这些宏，此时我们可以用这条   

```c
#ifdef _X86 
#pragma message(“_X86 macro activated!”) 
#endif 
当我们定义了_X86这个宏以后，应用程序在编译时就会在编译输出窗口里显示“_ 
X86 macro activated!”。我们就不会因为不记得自己定义的一些特定的宏而抓耳挠腮了 

```

#### progma 参数是code_seg
```C
#pragma code_seg( [ [ { push | pop}, ] [ identifier, ] ] [ "segment-name" [, "segment-class" ] ) 
```

该指令用来指定函数在.obj文件中存放的节,观察OBJ文件可以使用VC自带的dumpbin命令行程序,函数在.obj文件中默认的存放节 

* 为.text节：如果code_seg没有带参数的话,则函数存放在.text节中 

* push (可选参数) 将一个记录放到内部编译器的堆栈中,可选参数可以为一个标识符或者节名 

* pop(可选参数) 将一个记录从堆栈顶端弹出,该记录可以为一个标识符或者节名 

* identifier (可选参数) 当使用push指令时,为压入堆栈的记录指派的一个标识符,当该标识符被删除的时候和其相关的堆栈中的记录将被弹出堆栈 

```c

//默认情况下,函数被存放在.text节中 
void func1() { // stored in .text 
} 
//将函数存放在.my_data1节中 
#pragma code_seg(".my_data1") 
void func2() { // stored in my_data1 
} 
//r1为标识符,将函数放入.my_data2节中 
#pragma code_seg(push, r1, ".my_data2") 
void func3() { // stored in my_data2 
} 
int main() { 
} 
```

#### pragma once
`这是一个比较常用的指令,只要在头文件的最开始加入这条指令就能够保证头文件被编译一次.`



#### pragma hdrstop 
表示与编译头文件到此为止，后面的头文件不进行预编译

#### progma pack(n)
指定结构体对齐方式`#pragma pack(n)`来设定变量n字节对齐方式。n字节对齐就是说变量存放的起始地址的偏移量有梁中国默认情况：
* 如过n大于等于该变量所占用的字节数，那么偏移量必须满足迷人的对齐方式
* 如果n小于该变量的类型所占用的字节数,那么偏移量为n倍数，不用满足默认的对齐方式。  
结构的总大小也有个约束条件，分下面两种情况：如果大于所有成员变量类型所占用的字节数，  
那么结构的总大小必须为占用空间最大的变量占用的空间数的倍数：
	`否则必须为n的倍数`

```C
#pragma pack(push) //保存对齐状态
#pragma pack(4)//设定为4字节对齐
struct test
{
char m1;
double m4;
int m3;
}; 
#pragma pack(pop)//恢复对齐状态 
//为测试该功能，可以使用sizeof（）测试结构体的长度！
```



3. line
此命令主要是为强制编译器按指定的行号，开始对源程序的代码重新编号，在调试的时候，可以按此规程  
输出错误代码的准确位置。
```
# line constant “filename”:其作用是使得其后的源代码从指定的行号constant重新开始编号，并将当前文件的名命名为filename。例如下面的程序如下：
```


4. 运算符`#和##  #@`

```C
#define HI(x) printf("Hi,"#x"/n");
void main()
{
    HI(John);
}

```
程序的运行结果为：Hi John  
在预编译处理的时候，"#x"的作用是将x 替换为所带便的字符序列

```C
#define CONNECT(x,y) x##y
void main()
{
int a1,a2,a3;
CONNECT(a,1)=0;
CONNECT(a,2)=12;
a3=4;
printf("a1=%d/ta2=%d/ta3=%d",a1,a2,a3);
}

```


```C

#define CONNECT(x,y) x##y
void main()
{
int a1,a2,a3;
CONNECT(a,1)=0;
CONNECT(a,2)=12;
a3=4;
printf("a1=%d/ta2=%d/ta3=%d",a1,a2,a3);
}
```
程序的运行结果为：a1=0 a2=12 a3=4
在编译之前，CONNECT(a,1)被翻译为a1,
CONNECT(a,2)翻译为a2


5. 预定义常量
```c
__LINE__：当前源代码的行号，为整型常量 
__FILE__：当前编译程序文件的名称，为字符串 
__DATE__：编译程序文件日期，为字符串（”MM DD YYYY＂形式，如”　Jan 19 1993”） 
__TIME__：编译程序文件时间，为字符串（＂hh:mm:ss＂形式，如”08:30:23”） 
__STDC__：ANSI C标志，整型常量1,说明此程序兼容ANSI C标准。

```
