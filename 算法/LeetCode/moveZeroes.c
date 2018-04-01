void moveZeroes(int* nums, int numsSize) {
    
	int fast = 0,slow = 0;
	while(fast < numsSize)
	{
		if(nums[fast] != 0)
		{
			nums[slow] = nums[fast];
			++slow;
		}
		++fast;
	}
	for(int i = slow; i < numsSize; i++)
	{
		nums[i] = 0;
	}
    
}