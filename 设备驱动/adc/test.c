#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
int main(int argc, const char *argv[])
{
	int fd; 
	int data;
	fd = open("/dev/adc",O_RDWR);
	if(fd == -1)
	{
		perror("open");
		return -1;
	}

	while(1)
	{
		read(fd,&data,sizeof(data));
		printf("data = %d\n",data);
		sleep(1);
	}
	return 0;
}
