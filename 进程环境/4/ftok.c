#include<stdio.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/stat.h>
int main(int argc, const char *argv[])
{
	key_t key;

	key = ftok(".",'b');
	printf("key = %#x\n",key);

	struct stat info;
	stat(".",&info);
	printf("%#x,%#x\n",(unsigned int )info.st_dev,(unsigned int)info.st_ino);

	return 0;
}
