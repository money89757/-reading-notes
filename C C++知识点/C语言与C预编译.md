
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
1. #error:
语法格式如下：  
#error token-sequence
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