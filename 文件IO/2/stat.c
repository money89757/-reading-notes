#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>

int main(int argc, const char *argv[])
{
	struct stat info;
	stat("1.txt",&info);

	printf("%ld\n",info.st_ino);
	printf("%o\n",info.st_mode);
	printf("%d\n",info.st_nlink);
	return 0;
}
