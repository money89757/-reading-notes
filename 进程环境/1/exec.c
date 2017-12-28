#include<stdio.h>
#include<unistd.h>

int main(int argc, const char *argv[])
{
	printf("start\n");
#if 0
//	execl("/bin/ls","ls","-l","/home/linux/",NULL);

	execl("./1.sh","./1.sh",NULL);//注意：脚本要有执行权限，而且脚本中还要声明使用了哪种命令行解释器
	printf("hello\n");
#endif 

	char *const str[] = {"ls","-l","/home/linux",NULL};
	execv("/bin/ls",str);
	return 0;
}
