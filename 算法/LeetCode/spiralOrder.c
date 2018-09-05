#include <stdio.h>
#include <stdlib.h>


int *spiralOrder(int **matrix, int matrixRowSize, int matrixColSize)
{
	int top = 0;
	int bottom = matrixRowSize - 1;
	int left = 0;
	int right = matrixColSize - 1;
	int *nums = malloc(matrixRowSize * matrixColSize * sizeof(int));
	int count = 0;
	int i,direction = 0;

	while(top <= bottom && left <= right)
	{
		switch(direction)
		{
			case 0:
				for(i = left; i <= right; i++)
				{
					nums[count++] = matrix[top][i];
				}
				top++;
				break;
			case 1:
				for(i = top; i <= bottom; i++)
				{
					nums[count++] = matrix[i][right];
				}
				right--;
				break;
			case 2:
				for(i = right; i >= left; i--)
				{
					nums[count++] = matrix[bottom][i];
				}
				bottom--;
				break;
			case 3:
				for(i = bottom; i >= top; i--)
				{
					nums[count++] = matrix[i][left];
				}
				left++;
				break;
			default:
				break;
		}
		direction++;
		direction %= 4;
	}

	return nums;

#if 0

	if(matrixColSize == 0 || matrixColSize == 0) {
        return NULL;
    }
    int i = 0, j=0,num = 0;
    int top = 0,left = 0,right = matrixColSize-1,bottom = matrixRowSize-1;
    int type = 0;//0表示朝右,1表示朝下,2表示朝左,3表示朝上
    int *array = malloc(matrixRowSize * matrixColSize * sizeof(int));
    array[num++] = matrix[i][j];
    while(!(top == bottom && left == right)) {
        if(type == 0) {
            if(j+1 <= right) {
                ++j;
                if(top == bottom) {
                    ++left;
                }
            } else {
                type = 1;
                ++top;
                ++i;
            }
        } else if(type == 1) {
            if(i+1 <= bottom) {
                ++i;
                if(left == right) {
                    ++top;
                }
            } else {
                type = 2;
                --right;
                --j;
            }
        } else if(type == 2) {
            if(j-1 >= left) {
                --j;
                if(top == bottom) {
                    --right;
                }
            } else {
                type = 3;
                --bottom;
                --i;
            }
        } else if(type == 3) {
            if(i-1 >= top) {
                --i;
                if(left == right) {
                    --bottom;
                }
            } else {
                type = 0;
                ++left;
                ++j;
            }
        }
        array[num++] = matrix[i][j];
    }
    return array;
	
#endif

}


int main(int argc, const char *argv[])
{
	int i, j, count = 0;
	int row = 3;
	int col = 3;

	int **def = malloc(row * sizeof(int *));
	for(i = 0; i < row; i++)
	{
		def[i] = malloc(col * sizeof(int));
		for(j = 0; j <col; j++)
		{
			def[i][j] = ++count;
			printf("%d ",def[i][j]);
		}
		printf("\n");
	}

	int *nums = spiralOrder(def,row,col);
	for(i = 0; i < row * col; i++)
	{
		printf("%d ",nums[i]);
	}
	printf("\n");
	return 0;
}
