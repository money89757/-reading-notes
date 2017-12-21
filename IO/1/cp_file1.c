#include<stdio.h>

int main(int argc, const char *argv[])
{
	FILE *fpsrc;
	FILE *fpdest;

	if(argc < 3)
	{
		fprintf(stderr,"error\n");
		return -1;
	}

	fpsrc = fopen(argv[1],"r");
	if(fpsrc == NULL)
	{
		perror("fopen src");
		return -1;
	}

	fpdest = fopen(argv[2],"w");
	if(fpdest == NULL)
	{

	}

	size_t n;
	char buf[10];
	while(1)
	{
		n = fread(buf,1,10,fpsrc);

		if(n < 10 && feof(fpsrc) == 0)
		{
			printf("error\n");
			return -1;
		}

		if(n < 10 && feof(fpsrc) != 0)
		{
			fwrite(buf,1,n,fpdest);
			printf("到达文件尾部\n");
			break;
		}
		fwrite(buf,1,n,fpdest);

	}
	fclose(fpsrc);
	fclose(fpdest);
	return 0;
}
