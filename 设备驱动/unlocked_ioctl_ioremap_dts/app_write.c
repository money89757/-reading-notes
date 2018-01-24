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
		printf(">>");
		scanf("%s",buf);
		ret = write(fd,buf,strlen(buf));
		printf("write:ret:%d, buf:%s\n",ret,buf);
		memset(buf,0,sizeof(buf));
	}
	close(fd);
	printf("closed\n");
	return 0;
}
