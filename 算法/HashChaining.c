#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef __cplusplus
typedef char bool;
#define false 0
#define true 1
#endif 

typedef int hash_set_elem_t;

#define HASH_SET_CAPACITY 100001

#define PRIME 99997

typedef struct hash_set_t{
	int head[PRIME];

	struct{
		hash_set_elem_t elem;
		int next;
	}node[HASH_SET_CAPACITY];

	int size;

	int (*hash)(const hash_set_elem_t *elem);
	bool (*cmp)(const hash_set_elem_t *elem1,
				const hash_set_elem_t *elem2);
}hash_set_t;

hash_set_t * hash_set_create(int (*hash)(const hash_set_elem_t *elem),
		 bool (*cmp)(const hash_set_elem_t *elem1,
			const hash_set_elem_t *elem2))
{
	hash_set_t *result = (hash_set_t *)malloc(sizeof(hash_set_t));
	memset(result->head,-1,sizeof(result->head));
	memset(result->head,-1,sizeof(result->node));

	result->size = 0;
	result->hash = hash;
	result->cmp = cmp;

	return result;
}


void hash_set_destroy(hash_set_t *set)
{
	free(set);
	set = NULL;
}

bool hash_set_find(const hash_set_t *set, const hash_set_elem_t *elem)
{
	int i;
	int hash = set->hash(elem);

	for(i = set->head[hash]; i != -1; i = set->node[i].next)
	{
		if(set->cmp(elem,&(set->node[i].elem)))
		{
			return true;
		}
	}

	return false;

}

bool hash_set_add(hash_set_t *set, const hash_set_elem_t *elem)
{
	int i;

	int hash = set->hash(elem);

	for( i = set->head[hash]; i != -1; i = set->node[i].next)
	{
		if(set->cmp(elem,&(set->node[i].elem)))
		{
			return false;
		}
	}

	set->node[set->size].next = set->head[hash];
	set->node[set->size].elem = *elem;
	set->head[hash] = set->size++;
	return true;
}


