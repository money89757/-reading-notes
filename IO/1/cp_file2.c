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

	char *s;
	char buf[10];
	while(1)
	{
		s = fgets(buf,10,fpsrc);

		if(s == NULL && feof(fpsrc) == 0)
		{
			printf("error\n");
			return -1;
		}
		if(s == NULL && feof(fpsrc) != 0)
		{
			printf("尾部\n");
			break;
		}
		
		fputs(s,fpdest);
	}
	fclose(fpsrc);
	fclose(fpdest);
	return 0;
}
