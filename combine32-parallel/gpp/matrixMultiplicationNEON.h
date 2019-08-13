#include <stdio.h>
#include "Timer.h"
#include <arm_neon.h>

#define DATA_PER_INSTANCE 4

void matrixMultiplicationNEON(int SIZE, int32_t *mat1, int32_t *mat2, int32_t *prod);

void matrixMultiplicationNEONThreePart(int SIZE, int32_t *mat1, int32_t *mat2, int32_t *prod);