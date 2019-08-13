#include "matMult_NEON.h"

void generate_matrix(int SIZE, int16_t *mat1, int16_t *mat2)
{
	int i, j;
	
	for (i = 0;i < SIZE; i++)
	{
		for (j = 0; j < SIZE; j++)
		{
			mat1[SIZE*i + j] = i+j*2;
		}
	}
	
	for(i = 0; i < SIZE; i++)
	{
		for (j = 0; j < SIZE; j++)
		{
			mat2[SIZE*i + j] = i+j*3;
		}
	}	
}

void matMult(int SIZE, int16_t *mat1, int16_t *mat2, int16_t *prod)
{
	int i, j, k;
	for (i = 0;i < SIZE; i++)
	{
		for (j = 0; j < SIZE; j++)
		{
			prod[SIZE*i + j]=0;
			for(k=0;k<SIZE;k++)
				prod[SIZE*i + j] = prod[SIZE*i + j] +  mat1[i*SIZE + k] * mat2[k*SIZE + j];
		}
	}
}

char compareResults(int SIZE, int16_t *mat1, int16_t *mat2)
{
  int i, j;

	for (i = 0;i < SIZE; i++)
	{
		for (j = 0; j < SIZE; j++)
		{
			if (mat1[SIZE*i + j] != mat2[SIZE*i + j])
			{
        printf("Unfortunately both matrices are not same. \n");
				return 0;
			}
		}
	}	
	printf("Both matrices are same. Good job !! \n");
	return 1;
}

void printResults(int SIZE, int16_t *prod)
{
	int i, j;
	
	for (i = 0;i < SIZE; i++)
	{
		printf("\n");
		for (j = 0; j < SIZE; j++)
		{
			printf("\t%d ", prod[i*SIZE + j]);
		}
	}

	printf("\nDone !!! \n");	
}

void copyNEON(int SIZE, int16_t *matrix_in, int16x8_t *matrix_out)
{
  int i,j;
  
  for(i=0;i<SIZE;i++)
  {
    for(j=0;j<SIZE/DATAPERINS;j++)
    {
	    matrix_out[i*SIZE/DATAPERINS + j] = vld1q_s16(&matrix_in[i*SIZE + j*DATAPERINS]); 
    }
  }	
}

void initMatrix(int SIZE, int16x8_t *matrix)
{
  int i;
  
  for(i=0;i<SIZE*SIZE;i+=DATAPERINS)
  {
	  matrix[i] = vmovq_n_s16(0); 
  }	
}

void matMult_NEON(int SIZE, int16_t *mat1, int16_t *mat2, int16_t *prod)
{
  int i,j,k;
  int temp;
  int16x8_t line_one, line_two, result;
  
  // if (SIZE%DATAPERINS != 0)
  // {
	// SIZE_ceil = (SIZE/DATAPERINS) + SIZE;
	  
	// int16_t *prod2 = (int16_t *)memalign(sizeof(int16_t)*8, sizeof(int16_t)*SIZE_ceil*SIZE_ceil);
	// if(prod2==NULL){
		// printf("can't allocate the required memory for result_pl\n");
		// free(prod2);
		// return 0;
	// }
	
	// for(k=0; k<SIZE_ceil; k++)
	// {
		// for(i=0; i<SIZE_ceil; i+=DATAPERINS)
		// {
			// j = k*SIZE_ceil;
			// line_one = vmovq_n_s16(mat1[j]);  
			// line_two = vld1q_s16(&mat2[i]); 
			// result = vmulq_s16(line_one,line_two);	
			// for(j=j+1; j%SIZE_ceil!=0; j++)
			// {
				// line_one = vmovq_n_s16(mat1[j]);  
				// line_two = vld1q_s16(&mat2[i+(j%SIZE_ceil)*SIZE_ceil]); 
				// result = vaddq_s16(result,vmulq_s16(line_one,line_two));
			// }
			// vst1q_s16(&prod2[i+k*SIZE_ceil], result);  
		// }  
	// }
	
	// for (i = 0;i < SIZE; i++)
	// {
		// for (j = 0; j < SIZE; j++)
		// {
			// prod[SIZE*i + j] = prod2[SIZE*i + j];
		// }
	// }
	
	// free(prod2);
	
  // }
  // else 
  // {
	  for(k=0; k<SIZE; k++)
	  {
		for(i=0; i<SIZE; i+=DATAPERINS)
		{
			j = k*SIZE;
			line_one = vmovq_n_s16(mat1[j]);  
			line_two = vld1q_s16(&mat2[i]); 
			result = vmulq_s16(line_one,line_two);	
			for(j=j+1; j%SIZE!=0; j++)
			{
				line_one = vmovq_n_s16(mat1[j]);  
				line_two = vld1q_s16(&mat2[i+(j%SIZE)*SIZE]); 
				result = vaddq_s16(result,vmulq_s16(line_one,line_two));
			}
			vst1q_s16(&prod[i+k*SIZE], result);  
		}  
	  }	  
//  }
}

void moveNEONtoArm(int SIZE, int16x8_t *matrix_128, int16_t *matrix_arm)
{
  int i,j;
  
 	for (i = 0;i < SIZE; i++)
	{
		for (j = 0; j < SIZE/DATAPERINS; j++)
		{
			vst1q_s16(&matrix_arm[i*SIZE + j*DATAPERINS], matrix_128[i*SIZE/DATAPERINS + j]);
		}
	}	
}

void printResults_NEON(int SIZE, int16x8_t *matrix)
{
  int i,j;

 	for (i = 0;i < SIZE; i++)
	{
		printf("\n");
		for (j = 0; j < SIZE/DATAPERINS; j++)
		{
			printf("\t%d ", vgetq_lane_s16(matrix[i*SIZE/DATAPERINS + j],0));
      printf("\t%d ", vgetq_lane_s16(matrix[i*SIZE/DATAPERINS + j],1));
      printf("\t%d ", vgetq_lane_s16(matrix[i*SIZE/DATAPERINS + j],2));
      printf("\t%d ", vgetq_lane_s16(matrix[i*SIZE/DATAPERINS + j],3));
      printf("\t%d ", vgetq_lane_s16(matrix[i*SIZE/DATAPERINS + j],4));
      printf("\t%d ", vgetq_lane_s16(matrix[i*SIZE/DATAPERINS + j],5));
      printf("\t%d ", vgetq_lane_s16(matrix[i*SIZE/DATAPERINS + j],6));
      printf("\t%d ", vgetq_lane_s16(matrix[i*SIZE/DATAPERINS + j],7));
		}
	}
 
	printf("\nDone !!! \n");
}