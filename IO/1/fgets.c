#include<stdio.h>

int main(int argc, const char *argv[])
{
	FILE *fp;
	char buf[10];

	fp = fopen("1.txt","r");
//	gets(buf);
//	puts(buf);
	char *s;
	while(1)
	{
		s = fgets(buf,10,fp);

		if(s == NULL && feof(fp) == 0)
		{
			fprintf(stderr,"error\n");
			return -1;
		}
		if(s == NULL && feof(fp) != 0)
		{
			printf("到达文件尾部\n");
			break;
		}
//		printf("%s\n",buf);
		fputs(buf,stdout); // <==> printf("%s",s);	

	}
	
	fclose(fp);
	return 0;
}
