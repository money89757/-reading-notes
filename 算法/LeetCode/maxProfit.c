#include <stdio.h>

int maxProfit(int *prices, int pricesSize)
{
	int sum = 0;
	int i = 1;
	int diff;
	for(; i < pricesSize; i++)
	{

//		diff = prices[i] - prices[i-1];

//		if(diff > 0)
//			pro += diff;
		if(prices[i] > prices[i-1])
		{
			sum += prices[i] - prices[i-1];
		}
	}
	return sum;
}

int main(int argc, const char *argv[])
{
	int str1[5] = {1,2,3,4,5};
	int str2[7] = {2,3,2,1,3,0,5};
	int str3[1] ={1};
	int str4[5] = {5,4,3,2,1};

	int i,j,k,u;
	
	i = maxProfit(str1,5);
	j = maxProfit(str2,7);
	k = maxProfit(str3,1);
	u = maxProfit(str4,5);

	printf("%d %d %d %d\n",i,j,k,u);
	return 0;
}
