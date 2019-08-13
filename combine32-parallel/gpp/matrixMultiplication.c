#include "matrixMultiplication.h"
#include <string.h>

void generateMatrix(int SIZE, int32_t *mat1, int32_t *mat2)
{
	int i, j;
	
	for (i = 0;i < SIZE; i++)
	{
		for (j = 0; j < SIZE; j++)
		{
			mat1[SIZE*i + j] = 1;//i+j*2;
		}
	}
	
	for(i = 0; i < SIZE; i++)
	{
		for (j = 0; j < SIZE; j++)
		{
			mat2[SIZE*i + j] = 1;//i+j*3;
		}
	}	
}

void generateMatrixPadding(int SIZE, int SIZE_ceil, int32_t *mat1, int32_t *mat2)
{
	int i, j;
	
	for (i = 0;i < SIZE; i++)
	{
		for (j = 0; j < SIZE; j++)
		{
			mat1[SIZE_ceil*i + j] = i+j*2;
		}
	}
	
	for(i = 0; i < SIZE; i++)
	{
		for (j = 0; j < SIZE; j++)
		{
			mat2[SIZE_ceil*i + j] = i+j*3;
		}
	}	
}

void matrixMultiplication(int SIZE, int32_t *mat1, int32_t *mat2, int32_t *prod)
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

void matrixMultiplicationOnePart(int SIZE, int32_t *mat1, int32_t *mat2, int32_t *prod)
{
	int i, j, k;
	for (i = SIZE/4;i < SIZE; i++)
	{
		for (j = 0; j < SIZE; j++)
		{
			prod[SIZE*i + j]=0;
			for(k=0;k<SIZE;k++)
				prod[SIZE*i + j] = prod[SIZE*i + j] +  mat1[i*SIZE + k] * mat2[k*SIZE + j];
		}
	}
}	

void storeMatrixToBuffer(int SIZE, int32_t *mat1, int32_t *mat2, Uint32 *pool_notify_DataBuf)
{
	int i, j, startMatrix2;
	
	// fill in the first matrix to pool
	for (i = 0;i < SIZE; i++)
	{
		for (j = 0; j < SIZE; j++)
		{
			pool_notify_DataBuf[i*SIZE + j] =  (Uint32) mat1[i*SIZE + j];
		}
	}	
	
	// fill in the second matrix to pool
	startMatrix2=SIZE*SIZE;
	for (i = 0;i < SIZE; i++)
	{
		for (j = 0; j < SIZE; j++)
		{
			pool_notify_DataBuf[startMatrix2 + i*SIZE + j] =  (Uint32) mat2[i*SIZE + j];
		}
	}		
}

void printPoolBuffer(int SIZE, Uint32 *pool_notify_DataBuf)
{
	int i, j;
	
	for (i = 0;i < SIZE; i++)
	{
		printf("\n");
		for (j = 0; j < SIZE; j++)
		{
			printf("\t%d ", (int32_t)pool_notify_DataBuf[i*SIZE + j]);
		}
	}

	printf("\n Print the first matrix of pool buffer done !!! \n");		
	
	for (i = 0;i < SIZE; i++)
	{
		printf("\n");
		for (j = 0; j < SIZE; j++)
		{
			printf("\t%d ", (int32_t)pool_notify_DataBuf[SIZE*SIZE + i*SIZE + j]);
		}
	}	
	
	printf("\n Print the second matrix of pool buffer done !!! \n");		
}

void printResultsBuffer(int SIZE, Uint32 *pool_notify_DataBuf)// 
{
	int i, j;
	int start = 2*SIZE*SIZE;	

	printf("hellooo!! \n"); //

	for (i = 0;i < SIZE; i++)
	{	
		printf("\n");
		for (j = 0; j < SIZE; j++)
		{
			// printf("\t%d ",prod[i*SIZE + j]); //
			printf("\t%d ", (int32_t)pool_notify_DataBuf[start + i*SIZE + j]);//  
		}
	}

	printf("\nMatrix Multiplication Results. Done !!! \n");	
}

void printResults(int SIZE, int32_t *matrix)// 
{
	int i, j;
	
	for (i = 0;i < SIZE; i++)
	{
		for (j = 0; j < SIZE; j++)
		{
			printf("%d \t",matrix[i*SIZE + j]); //
		}
		printf("\n");
	}
	
}

char compareResults(int SIZE, int32_t *mat1, int32_t *mat2)
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

int saveResultToFile(char * name, int32_t * matrix, int SIZE)
{
	FILE *fptr;
	char there_was_error = 0;
	char opened_in_read  = 1;
	int i, j;

	strcat(name, ".csv");
	fptr = fopen(name, "rb+");
    if(fptr == NULL) //if file does not exist, create it
    {
    	opened_in_read = 0;
    	fptr = fopen(name, "wb");
    	if (fptr == NULL)
    		there_was_error = 1;
    }
    if (there_was_error)
    {
    	printf("Disc full or no permission\n");
    	return EXIT_FAILURE;
    }
    // if (opened_in_read)
    //     printf("The file is opened in read mode."
    //            " Let's read some cached data\n");
    // else
    //     printf("The file is opened in write mode."
    //            " Let's do some processing and cache the results\n");


    for (i = 0;i < SIZE; i++)
    {
    	for (j = 0; j < SIZE; j++)
    	{
			fprintf(fptr, "%d \t",matrix[i*SIZE + j]); //
		}
		fprintf(fptr, "\n");
	}

	return EXIT_SUCCESS;
}

void initMatrix(int SIZE, int32_t *matrix)
{
  int i;
  
  for(i=0;i<SIZE*SIZE;i++)
  {
	  matrix[i] = 0; 
  }	
}