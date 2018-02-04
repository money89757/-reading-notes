#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define FIFO_SERVER "/tmp/myfifo"

int main(int argc,char** argv)
{
	int fd;
	char w_buf[100];
	int nwrite;
	
    /*创建有名管道*/
	if((mkfifo(FIFO_SERVER,O_CREAT|O_EXCL|O_RDWR)<0)&&(errno!=EEXIST))
		printf("cannot create fifoserver\n");

	/*打开管道*/
	fd=open(FIFO_SERVER,O_RDWR|O_NONBLOCK,0);
	if(fd==-1)
	{
	    perror("open");
		exit(1);
	}
	
	/*入参检测*/
	if(argc==1)
	{
		printf("Please send something\n");
		exit(-1);
	}
	strcpy(w_buf,argv[1]);
	
	/* 向管道写入数据 */
	if((nwrite=write(fd,w_buf,100))==-1)
	{
		if(errno==EAGAIN)
			printf("The FIFO has not been read yet.Please try later\n");
	}
	else 
		printf("write %s to the FIFO\n",w_buf);
	close(fd); //关闭管道
	return 0;
}

