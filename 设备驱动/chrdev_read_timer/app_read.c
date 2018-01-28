#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define GLOBALSIZE 128
#define PATHNAME "/dev/ex"

int main(int argc, const char *argv[])
{
	int fd = -1;
	unsigned int seconds,old_seconds;
	int ret = 0;
	fd = open(PATHNAME,O_RDWR);
	if(fd < 0){
		perror("open failed");
		return -1;
	}
	printf("open has done!\n");
	while(1){
		ret = read(fd,&seconds,sizeof(unsigned int));
		if(old_seconds != seconds){
			printf("after linux work ,seconds: %d\n",seconds);
			old_seconds = seconds;
		}
	}
	close(fd);
	printf("closed\n");
	return 0;
}
