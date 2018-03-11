ONE:


#include <stdio.h>

int main (void)
{
	chararray[100];

	printf("输入您要打印的内容：");
	sacnf("%[^ ]",array);

	printf("您输入的内容为：%s\n",array );

	return 0;
}


TWO:

#include <stdio.h>

int main (void)
{
	char array[100];

	printf("输入要打印的内容：" );
	sacnf("%s",array);

	printf("您输入的内容为：%s\n", array);

	return 0;
}

解读：第一个程序使用的scanf(“%[^ ]”, array)是读入字符串直到遇到空格结束。第二个程序使用的是scanf(“%s”, array);也是遇到空格就结束。

比如输入：abcdefg abcdefg abcdefg

输出结果：abcdefg

THREE:

#include <stdio.h>  
   
int main( void )  
{  
         chararray[ 100 ];  
          
         printf("输入要打印的内容:" );  
         scanf("%[^\n]", array );   
          
         printf("您输入的内容为: %s\n", array );  
            
         return0;  
}  

解读：本程序读取字符直到遇到’\n’或回车为止。

比如输入：abcdefg abcdefg abcedef

输出结果：abcdefg abcdefg abcedef

PS：输出字符串中有\n不是换行符，只能作为字符串中的一部分处理。

比如输入：abcdefg\n abcdefg abcdefg

输出结果：abcdefg\n abcdefg abcdefg

总结：

综合上述，我们可以知道%[^\n]和%[^ ]的含义了。符号^ 表示取反的意思。[^ ]表示除了空格，所有的字符都可以读取；[^\n]则表示除了换行符，所有的字符都可以读取。

有时候，在程序中需要读取字符串直到回车为止，也可以使用函数gets(array)读取。


