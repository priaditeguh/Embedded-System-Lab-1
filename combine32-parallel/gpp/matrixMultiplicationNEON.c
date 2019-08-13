#include "matrixMultiplicationNEON.h"

void matrixMultiplicationNEON(int SIZE, int32_t *mat1, int32_t *mat2, int32_t *prod)
{
	int i,j,k;
	int32x4_t line_one, line_two, result;

	for(k=0; k<SIZE; k++)
	{
		for(i=0; i<SIZE; i+=DATA_PER_INSTANCE)
		{
			j = k*SIZE;
			line_one = vmovq_n_s32(mat1[j]);
			line_two = vld1q_s32(&mat2[i]);
			result = vmulq_s32(line_one,line_two);
			for(j=j+1; j%SIZE!=0; j++)
			{
				line_one = vmovq_n_s32(mat1[j]);
				line_two = vld1q_s32(&mat2[i+(j%SIZE)*SIZE]);
				result = vaddq_s32(result,vmulq_s32(line_one,line_two));
			}
			vst1q_s32(&prod[i+k*SIZE], result);
		}  
	}	  
}

void matrixMultiplicationNEONThreePart(int SIZE, int32_t *mat1, int32_t *mat2, int32_t *prod)
{
	int i,j,k;
	int32x4_t line_one, line_two, result;

	for(k=SIZE/4; k<SIZE; k++)
	{
		for(i=0; i<SIZE; i+=DATA_PER_INSTANCE)
		{
			j = k*SIZE;
			line_one = vmovq_n_s32(mat1[j]);
			line_two = vld1q_s32(&mat2[i]);
			result = vmulq_s32(line_one,line_two);
			for(j=j+1; j%SIZE!=0; j++)
			{
				line_one = vmovq_n_s32(mat1[j]);
				line_two = vld1q_s32(&mat2[i+(j%SIZE)*SIZE]);
				result = vaddq_s32(result,vmulq_s32(line_one,line_two));
			}
			vst1q_s32(&prod[i+k*SIZE], result);
		}  
	}	  
}