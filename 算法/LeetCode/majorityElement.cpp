#include <stdio.h>

int majorityElement(int* nums, int numsSize) {
          
        int date = 0;  
        int times = 0;  
        for(int i = 0; i < numsSize; i++){  
            if(times == 0) date = nums[i];  
            if(nums[i] != date){  
                times--;  
            } else{  
                times++;  
            }  
        }  
        return date;  
    
}

int main(void)
{
	int str1[3] = {3,2,3};
	int str2[7] = {2,2,1,1,1,2,2};
	
	int n = majorityElement(str1,3);
	int m = majorityElement(str2,7);
	
	printf("str1 = %d\n",n);
	printf("str2 = %d\n",m);
}
