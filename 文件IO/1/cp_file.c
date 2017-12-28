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

	int ch;
	while(1)
	{
		ch = fgetc(fpsrc);

		if(ch == EOF && feof(fpsrc) == 0)
		{
			fprintf(stderr,"fpsrc error\n");
			return -1;
		}

		if(ch == EOF && feof(fpsrc) != 0)
		{
			printf("文件到达尾部\n");
			break;
		}
		fputc(ch,fpdest);
	}
	fclose(fpsrc);
	fclose(fpdest);
	return 0;
}
