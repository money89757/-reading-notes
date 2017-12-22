#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>
int main(int argc, const char *argv[])
{
	int fd;

	fd = open("1.txt",O_RDONLY | O_CREAT | O_EXCL,0664);
#if 0
	if(fd == -1)
	{
		printf("errno = %d\n",errno);//全局变量，可以直接使用，但是要加errno.h头文件
									//代表了全局错误码
		perror("open");
	}
#endif 
	if(fd == -1)
	{
		if(errno == EEXIST)//说明文件实现已经存在，当前虽然出错，但是我们可以直接打开文件
		{
			fd = open("1.txt",O_RDONLY);
			//操作
		}
		else 
		{
			perror("open");//正常报错退出
			return -1;
		}
	}
	else //说明文件事先不存在，我们当前程序创建的,这个位置说明open正常执行了。也说明文件已经被打开
	{
		//操作
	}
	return 0;
}
