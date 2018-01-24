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
	int ret = 0;
	fd = open(PATHNAME,O_RDWR);
	char buf[GLOBALSIZE] = {};
	if(fd < 0){
		perror("open failed");
		return -1;
	}
	printf("open has done!\n");
	while(1){
		memset(buf,0,sizeof(buf));
		ret = read(fd, buf,2);
		printf("read:ret:%d, buf:%s\n",ret,buf);
		sleep(1);
	}
	close(fd);
	printf("closed\n");
	return 0;
}
