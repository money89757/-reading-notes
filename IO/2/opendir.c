#include<stdio.h>
#include<sys/types.h>
#include<dirent.h>

int main(int argc, const char *argv[])
{
	DIR *dirp;
	struct dirent *pdirent;
	dirp = opendir("/home/linux");

	if(dirp == NULL)
	{
		perror("opendir");
		return -1;
	}

//	pdirent = readdir(dirp);

//	printf("%s\n",pdirent->d_name);
	
	chdir("/home/linux");// 等价于 cd /home/linux/

	int count = 0;
	while((pdirent = readdir(dirp)) != NULL)
	{
		if(pdirent->d_name[0] == '.')
			continue;
		count ++;
		printf("%s\t",pdirent->d_name);
		if(count % 5 == 0)
			printf("\n");
	}
	printf("\n");
	return 0;
}
