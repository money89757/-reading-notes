#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

#define PATHNAME "/dev/ex"

int main(int argc, const char *argv[])
{
	int fd = -1;
	fd = open(PATHNAME,O_RDWR);
	if(fd < 0){
		perror("open failed");
		return -1;
	}
	printf("open has done!\n");
	close(fd);
	return 0;
}
