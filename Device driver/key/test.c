#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
int main(int argc, const char *argv[])
{
	int fd; 
	int key;
	fd = open("/dev/key",O_RDWR);
	if(fd == -1)
	{
		perror("open");
		return -1;
	}

	while(1)
	{
		read(fd,&key,sizeof(key));
		printf("key = %d\n",key);
	}
	return 0;
}
