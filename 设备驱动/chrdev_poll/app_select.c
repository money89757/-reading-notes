#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/select.h>

#define GLOBALSIZE 128
#define PATHNAME "/dev/ex"

int main(int argc, const char *argv[])
{
	int fd = -1;
	int ret = 0;
	fd_set fr,fw;

	fd = open(PATHNAME,O_RDWR);
	char buf[GLOBALSIZE] = {};
	if(fd < 0){
		perror("open failed");
		return -1;
	}
	printf("open has done!\n");
	while(1){
		FD_ZERO(&fr);
		FD_ZERO(&fw);
		FD_SET(fd,&fr);
		FD_SET(fd,&fw);
		//实现select操作
		if(select(fd+1,&fr,&fw,NULL,NULL) < 0){
			perror("select failed");
			return -1;
		}else{
			if(FD_ISSET(fd,&fr)){
				puts("read available");
			}
			if(FD_ISSET(fd,&fw)){
				puts("write available");
			}
		}
	}
	close(fd);
	printf("closed\n");
	return 0;
}
