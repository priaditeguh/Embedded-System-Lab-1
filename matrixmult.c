#include<stdio.h>
#include "Timer.h"

// #define SIZE 16

// void matMult(int mat1[SIZE][SIZE], int mat2[SIZE][SIZE], int prod[SIZE][SIZE]);
void matMult(int ** mat1, int ** mat2, int ** prod, int SIZE);

int main(int argc, char ** argv)
{
	printf("%d\n", argc);
	printf("%s\n", argv[0]);
	printf("%s\n", argv[1]);
	
	int SIZE = 128;
	if (argc > 1) {
		SIZE = atoi(argv[1]);
	}

    Timer totalTime;
    initTimer(&totalTime, "Total Time");

	// int mat1[SIZE][SIZE], mat2[SIZE][SIZE], prod[SIZE][SIZE];

	int **mat1 = malloc(SIZE * sizeof(int *));
	for(int i = 0; i < SIZE; i++)
		mat1[i] = malloc(SIZE * sizeof(int));
	int **mat2 = malloc(SIZE * sizeof(int *));
	for(int i = 0; i < SIZE; i++)
		mat2[i] = malloc(SIZE * sizeof(int));
	int **prod = malloc(SIZE * sizeof(int *));
	for(int i = 0; i < SIZE; i++)
		prod[i] = malloc(SIZE * sizeof(int));
	
	// int * mat1, mat2, mat3;
	// mat1 = (int *)malloc(SIZE * SIZE * sizeof(int));
	// mat2 = (int *)malloc(SIZE * SIZE * sizeof(int));
	// mat3 = (int *)malloc(SIZE * SIZE * sizeof(int));
	
	int i, j;
	
	for (i = 0;i < SIZE; i++)
	{
		for (j = 0; j < SIZE; j++)
		{
			mat1[i][j] = i+j*2;
		}
	}
	
	for(i = 0; i < SIZE; i++)
	{
		for (j = 0; j < SIZE; j++)
		{
			mat2[i][j] = i+j*3;
		}
	}

    startTimer(&totalTime);
	matMult(mat1,mat2,prod,SIZE);
    stopTimer(&totalTime);
    printTimer(&totalTime);	

	for (i = 0;i < SIZE; i++)
	{
		printf("\n");
		for (j = 0; j < SIZE; j++)
		{
			printf("\t%d ", prod[i][j]);
		}
	}
	
	for(int i = 0; i < SIZE; i++){
		free(mat1[i]);
		free(mat2[i]);
		free(prod[i]);
	}

	free(mat1);
	free(mat2);
	free(prod);

	printf("\nDone !!! \n");
	return 0;
}

void matMult(int ** mat1, int ** mat2, int ** prod, int SIZE)
{
	int i, j, k;
	for (i = 0;i < SIZE; i++)
	{
		for (j = 0; j < SIZE; j++)
		{
			prod[i][j]=0;
			for(k=0;k<SIZE;k++)
				prod[i][j] = prod[i][j]+mat1[i][k] * mat2[k][j];
		}
	}
}
