#include <stdio.h>
#include <stdlib.h>


typedef struct Node
{
	int data;
	struct Node *pNext;

}NODE,*PNODE;


PNODE Create_list(void)
{
	int len;
	int il
	int val;

	PNODE List;
	PNODE pHead = (PNODE)malloc(sizeof(NODE));

	if(NULL == pHead)
	{
		printf("Memory allocation failure");
		exit(-1);
	}

	else
	{
		PNODE pTail = pHead;
		pHead->pNext = NULL;
		printf("please input the length of list :");
		scanf("%d",&len);
		for(i=0; i<len; i++)
		{
			PNODE p = (PNODE)malloc(sizeof(NODE));
			if(NULL == p)
			{
				printf("Memory allocation failure");
				exit(-1;)
			}

			else
			{
				printf("please input the value of list:");
				scanf("%d",&val);
				p->data=val;
				pTail->pNext=p;
				p->pNext=NULL;
				pTail=p;
			}
		}
	}

	return pHead;
}


int main(int argc, const char *argv[])
{


	return 0;
}
