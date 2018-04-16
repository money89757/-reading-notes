#include <stdio.h>

int removeDuplicates(int *nums, int numsSize)
{
        if (nums == NULL || numsSize == 0) {
            return 0;
        }

        int i = 0;
        int j = 0;

        for (; i < numsSize; i++) {
            if (nums[j] != nums[i]) {
                nums[++j] = nums[i];
            }
        }
        return j+1;
}

int main()
{
	int str[3] = {1,1,2};
	
	int num = removeDuplicates(str,3);
	
	printf("%d \n",num);
}
