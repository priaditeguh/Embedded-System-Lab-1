#include<stdio.h>
#include "Timer.h"
#include <arm_neon.h>

#define DATAPERINS 8

void printResults(int SIZE, int16_t *prod);

void generate_matrix(int SIZE, int16_t *matrix_one, int16_t *matrix_two);

void matMult(int SIZE, int16_t *mat1, int16_t *mat2, int16_t *prod);

char compareResults(int SIZE, int16_t *mat1, int16_t *mat2);

void matMult_NEON(int SIZE, int16_t *mat1, int16_t *mat2, int16_t *prod);

void copyNEON(int SIZE, int16_t *matrix_in, int16x8_t *matrix_out);

void initMatrix(int SIZE, int16x8_t *matrix);

void printResults_NEON(int SIZE, int16x8_t *matrix);