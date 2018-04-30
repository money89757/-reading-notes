#include <stdio.h>
#include <stdlib.h>

typedef struct{
	int num;
	int min_idx;
	int stack[];
}MinStack;


MinStack* minStackCreate(int maxSize)
{
	MinStack *obj = (MinStack *)malloc(sizeof(MinStack) + sizeof(int) * maxSize);
	obj->num = 0;
	obj->min_idx = 0;
	return obj;
}

void minStackPush(MinStack* obj, int x)
{
	if(obj->num > 0 && x < obj->stack[obj->min_idx])
	{
		obj->min_idx = obj -> num;
	}
	obj->stack[obj->num++] = x;
}

void minStackPop(MinStack *obj)
{
	int i;
	if(--obj->num == obj->min_idx)
	{
		int min_idx = 0;
		for(i = 1; i < obj->num; i++)
		{
			if(obj->stack[i] < obj -> stack[min_idx])
			{
				min_idx = i;
			}
		}
		obj->min_idx = min_idx;
	}
}

int minStackTop(MinStack *obj)
{
	return obj->stack[obj->num - 1];
}

int minStackGetMin(MinStack *obj)
{
	return obj->stack[obj->min_idx];
}

void minStackFree(MinStack *obj)
{
	free(obj);
}

int main(void)
{
	MinStack *obj = minStackCreate(5);
	minStackPush(obj,2);
	minStackPush(obj,0);
	minStackPush(obj,3);
	minStackPush(obj,0);
	
	printf("Min:%d\n",minStackGetMin(obj));
	minStackPop(obj);
	printf("Min:%d\n",minStackGetMin(obj));
	minStackPop(obj);
	printf("Min:%d\n",minStackGetMin(obj));
	
	minStackPop(obj);
    printf("Min:%d\n", minStackGetMin(obj));
    minStackFree(obj);
}
