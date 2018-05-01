```c
#include <iostream>
using namespace std;

int main()
{
	http://www.baidu.com
	cout<<"hello world"<<endl;
	return 0;
}
```

带标签的语句是一种特殊的语句，  
在语句前面有一个标识符（即标签，上段代码中的http）和一个冒号。  
使用gotolabel就可以跳到标签处执行，比如可以在代码中写goto http，这样就会执行cout语句了。

case就是一种标签，case关键字和它对应的值一起，称为case标签。

### 类中的public、private、protect也是标签，称为成员访问标签。

## 标签必须是整型常量表达式
`整型常量` 不满足这个特性的不能作为case值，编译器会报错  

整型，意味着浮点数是不合法的，如case 3.14:不可以；常量，意味着变量是不合法的，如case ival: ival不能是变量。  

1. C++中的const int，注意仅限于C++中的const，C中的const是只读变量，不是常量；

2. 单个字符，如case 'a': 是合法的，因为文字字符是常量，被转成ASCII码，为整型；

3. 使用#define定义的整型，#define定义的一般为常量，比如#define pi 3.14，但是也必须是整型才可以；

4. 使用enum定义的枚举成员。因为枚举成员是const的，且为整型。如果不手动指定枚举值，则默认枚举值为从0开始，依次加

转自https://blog.csdn.net/szchtx/article/details/21647159