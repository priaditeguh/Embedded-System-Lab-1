#if !defined (matrixmult_H)
#define matrixmult_H 

/*  ----------------------------------- OS Specific Headers           */
#include <stdio.h>
#include <stdlib.h>

#include "pool_notify.h"

void generateMatrix(int SIZE, int32_t *mat1, int32_t *mat2);

void generateMatrixPadding(int SIZE, int SIZE_ceil, int32_t *mat1, int32_t *mat2);

void matrixMultiplication(int SIZE, int32_t *mat1, int32_t *mat2, int32_t *prod);

void matrixMultiplicationOnePart(int SIZE, int32_t *mat1, int32_t *mat2, int32_t *prod);

void storeMatrixToBuffer(int SIZE, int32_t *mat1, int32_t *mat2, Uint32 *pool_notify_DataBuf);

void printPoolBuffer(int SIZE, Uint32 *pool_notify_DataBuf);

void printResultsBuffer(int SIZE, Uint32 *pool_notify_DataBuf);

void printResults(int SIZE, int32_t *matrix);

char compareResults(int SIZE, int32_t *mat1, int32_t *mat2);

int saveResultToFile(char * name, int32_t * matrix, int SIZE);

void initMatrix(int SIZE, int32_t *matrix);

#endif 