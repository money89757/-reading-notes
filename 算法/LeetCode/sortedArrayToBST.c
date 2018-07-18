#include <stdio.h>
#include <stdlib.h>

struct TreeNode{
	int val;
	struct TreeNode *left;
	struct TreeNode *right;
};

struct TreeNode *partition(int *num, int l,int h)
{
	int mid = l + (h - l) / 2;
	struct TreeNode *node = malloc(sizeof(*node));
	node->val = num[mid];
	node->left = mid > l ? partition(num, l, mid - 1) : NULL;
	node->right = mid < h ? partition(num, mid + 1, h) : NULL;

	return node;
}

struct TreeNode* sortedArrayToBST(int *nums, int numsSize)
{
	if(numsSize == 0)
	{
		return NULL;
	}

	return partition(nums, 0 , numsSize - 1);
}

int main(int argc, const char *argv[])
{
	int i, count = argc - 1;
	int *nums = malloc(count * sizeof(int));

	for(i = 0; i < count; i++)
	{
		nums[i] = atoi(argv[i + 1]);
	}
	sortedArrayToBST(nums,count);
	return 0;
}
