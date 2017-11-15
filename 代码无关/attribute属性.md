# GNC C 中的 __attribute__ 机制

## __attribute__  机制

__attribute__ 是GCC提供的一种语法，可以帮助我们在编译时对声明的函数、变量和类型做一些特殊处理或者是操作检查。

__attribute__的语法格式为   `__attribute__((attribute-list))`
 ,attribute-list是指令集，attribute出现在函数、变量、和声明类型的 `；`前。

## 一、函数属性
#### 1.  format(archetype,string-index,first-to-check)

> format属性通过printf,scanf,strftime或strfmon等方法来检验函数的参数是否同样适用用于这些指定的格式化字符串方法，如果不适用，编译器在编译时的就会发出警报，从而发生错误。

```c
extern int
my_printf(int value,const char *my_format,...)
__attribute__ ((format(printf，2,3)))；

void main()
{
    my_printf(0,"age = %d\n",17);
    my_printf(0,"age = %d\n","17");
    my_printf(0,"age = %d name = %s\n",17,"bob");
}
```
上面示例中，format属性的第一个参数指定了一个printf方法，第二个参数string-index表示函数名my_printf里格式化参数是总参数的第几个，这里我们的格式化参数`my_format`第二个参数，format属性的第三个参数表示，参数集合`(...)`从函数my_printf的第几个参数开始出现。

#### 2. noreturn

> noreturn属性表示其指定的函数没有返回值，当编译器执行到这时，要面对现实，不要大惊小怪。下面是fatal的部分代码，在程序出错的位置调用fata函数打印信息，然后直接退出程序，不需要return。

```c
void fatal() __attribute__ ((noreturn));

void fatal(/* arguments */) {

  exit（1);
}
```

#### 3.deprecated
> deprecated属性可以用来标识一个预计将会被弃用的函数，如果开发者使用该函数，编译时就会发生警告，并提示出错的行数。警告信息只会在开发者调用该函数时才会提示：

```c
int old_fn() __attribute__ ((deprecated));
int old_fn();
int (* fn_ptr)() = old_fn;

```
在上面示例中只会在第3行提出警告

deprecated也可以用于`变量`和`类型`。

#### 4.constructor & destructor

设置constructor属性可以使函数在main方法之前执行，而设置destructor可以使函数在main方法之后执行
 ```c
#include <stdio.h>
#include <stdlib.h>

__attribute__((constructor)) void before_func()
{
  printf("before_func\n");
}

__attribute__((destructor)) void after_func()
{
  printf("after_func\n");

}

int main()
{
  printf("hello world\n");
  return 0;
}
 ```
## 二、变量属性
#### 1. aligned(alignment)
> aligned 属性让其指定的变量或结构体成员按 alignment 字节大小对齐。如果其中对齐长度有长度大于 alignment的,则按照最大对齐长度来对齐。

```C
#include <stdio.h>
#include <stdlib.h>

struct foo{
  char a;
  int x[2] __attribute__((aligned(8)));
};

int main()
{
  int s0 = sizeof(struct foo);
  printf("s0 = %d\n",s0);//printf s0 = 16;
  return 0;
}

```

#### 2. packed
>packed属性用于设置变量或结构体成员以最小的对齐方式对齐。在下列结构体中，由于x已经使用packed进行对齐，所以此时结构体以a的size来对齐。

```c
#include <stdio.h>
#include <stdlib.h>

struct foo{
  char a;
  int x[2] __attribute__ ((packed));
};

int main()
{
    int s0 = sizeof(struct foo);
    printf("s0 = %d\n",foo);//printf s0 = 9;
    return 0;
}

```

## 三、类型属性

#### 1. packed
>如果 packed 属性用在 struct 或 union 上,表示该结构的成员变量按照紧凑模式对齐,即以变量的实际占用字节对齐,不用编译器进行优化对齐。如果用在 enum 上,则表示使用最小的整数来存储枚举类型。

```c
#include <stdio.h>
#include <stdlib.h>

struct my_unpacked_struct()
{
    char c;
    int i;
};

struct __attribute__((__packed__)) my_packed_struct{

    char c;
    int i;
    struct my_unpacked_struct s;
};

int main()
{
    int s0 = sizeof(struct my_unpacked_struct);
    int s1 = sizeof(struct my_packed_struct);
    printf("s0 = %d  s1 = %d\n",s0,s1);//print s0 = 8;s1 = 13
    return 0;
}

```
