#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, const char *argv[])
{
	int fd;
	int nbyte;
	char buf[1024] = "Hello World!";

	fd = open("/dev/hello", O_RDWR);
	if (fd < 0) {
		perror("open");
		exit(1);
	}

	nbyte = write(fd, buf, strlen(buf));

	printf("nbyte = %d\n", nbyte);

	memset(buf, 0, sizeof(buf));

	read(fd, buf, sizeof(buf));

	printf("buf = %s\n", buf);

	close(fd);
	return 0;
}
