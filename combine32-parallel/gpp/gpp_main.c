/*  ----------------------------------- OS Specific Headers           */
#include <stdio.h>
#include <stdlib.h>
#include "matrixMultiplicationNEON.h"
#include "Timer.h"
#include <arm_neon.h>

/*  ----------------------------------- DSP/BIOS Link                 */
#include <dsplink.h>

/*  ----------------------------------- Application Header            */
#include <pool_notify.h>

/** ============================================================================
 *  @func   main
 *
 *  @desc   Entry point for the application
 *
 *  @modif  None
 *  ============================================================================
 */
int main (int argc, char ** argv)
{
  int32_t SIZE = 0;
  int32_t SIZE_ceil;
  int32_t *matrixA = NULL;
  int32_t *matrixB = NULL;
  int32_t *matrixResultARM = NULL;
  int32_t *matrixResult = NULL;

  #undef DEBUG

  Char8 * dspExecutable    = NULL ;
  
  long long timeImplementation = 0, timeARM;
  int option = 0; // 0=USE ONLY NEON, 1=USE PARALLEL NEON AND DSP POOL

  if (argc < 4) {
    printf ("Usage : %s <absolute path of DSP executable> "
     "<matrix size> <option: 0=USE ONLY NEON, 1=USE PARALLEL(NEON AND DSP POOL)>\n", 
     argv [0]) ;
  } else {
    SIZE = atoi(argv[2]); 
    if (SIZE > 128) {
      printf("This version implements for ""size <= 128"" only\n");
      return 0; 
    }

    if (argc>3){
      option=atoi(argv[3]);
      if (option != 0 && option != 1) {
        printf("Please use valid option. 0=USE ONLY NEON, 1=USE PARALLEL(NEON AND DSP POOL)\n");
        return 0;    
      } else {
        if (option==0)
          printf("Using only NEON \n");
        else if (option==1)
          printf("Using parallel solution (NEON and DSP pool)\n");
    
      }
    }

    if (SIZE%DATA_PER_INSTANCE != 0)
    {
      SIZE_ceil = (SIZE/DATA_PER_INSTANCE)*DATA_PER_INSTANCE + DATA_PER_INSTANCE;
    } else {
      SIZE_ceil = SIZE;
    } 
    
    // initialize the matrix
    matrixA = (int32_t*) memalign (sizeof(int32_t)*DATA_PER_INSTANCE, SIZE_ceil*SIZE_ceil * sizeof(int32_t) );
    if(matrixA==NULL){
      printf("can't allocate the required memory for matrix\n");
      return 0;
    } 

    matrixB = (int32_t*) memalign (sizeof(int32_t)*DATA_PER_INSTANCE, SIZE_ceil*SIZE_ceil * sizeof(int32_t) );
    if(matrixB==NULL){
      printf("can't allocate the required memory for matrix\n");
      free(matrixB);
      return 0;
    } 

    matrixResultARM = (int32_t*) memalign (sizeof(int32_t)*DATA_PER_INSTANCE, SIZE_ceil*SIZE_ceil * sizeof(int32_t) );
    if(matrixResultARM==NULL){
      printf("can't allocate the required memory for result_pl\n");
      free(matrixA);
      free(matrixB);
      return 0;
    }

    matrixResult = (int32_t*) memalign (sizeof(int32_t)*DATA_PER_INSTANCE, SIZE_ceil*SIZE_ceil * sizeof(int32_t) );
    if(matrixResult==NULL){
      printf("can't allocate the required memory for result_pl\n");
      free(matrixA);
      free(matrixB);
      free(matrixResultARM);
      return 0;
    }

    // generate the matrux
    initMatrix(SIZE_ceil, matrixA);
    initMatrix(SIZE_ceil, matrixB);
    initMatrix(SIZE_ceil, matrixResult);
    initMatrix(SIZE_ceil, matrixResultARM);
    // generateMatrix(SIZE,matrixA,matrixB);
    generateMatrixPadding(SIZE, SIZE_ceil, matrixA,matrixB);

    // matrix multiplication
    printf("ARM Matrix Multiplication\n");
    timeARM = get_usec();
    // startTimer(&totalTime);
    matrixMultiplication(SIZE_ceil,matrixA,matrixB,matrixResultARM);
    // stopTimer(&totalTime);
    // printTimer(&totalTime); 
    printf("Execution time %lld us.\n", get_usec()-timeARM);
    
    printf("Implementation Matrix Multiplication\n");
    if (option == 1) {
      // run the pool for computing
      dspExecutable = argv [1] ;

      pool_notify_Main(dspExecutable, SIZE_ceil, matrixA, matrixB, matrixResult, &timeImplementation);
      printf("Execution time %lld us.\n", timeImplementation);
    } else if (option == 0){
      timeImplementation = get_usec();
      matrixMultiplicationNEON(SIZE_ceil, matrixA, matrixB, matrixResult);
      printf("Execution time %lld us.\n", get_usec()-timeImplementation);
    }

    // #ifdef DEBUG
    // printf("SIZE: %d\n",SIZE);
    // printf("Matrix A \n");
    // printResults(SIZE, matrixA);
    // printf("Matrix B \n");
    // printResults(SIZE, matrixB);
    // printf("RESULT ARM \n");
    // printResults(SIZE, matrixResultARM);
    // printf("RESULT \n");
    // printResults(SIZE, matrixResult);
    // #endif
    
    // compare the result
    compareResults(SIZE, matrixResult, matrixResultARM);

    // saveResultToFile("ARM", matrixResultARM, SIZE);
    // saveResultToFile("Implementation", matrixResult, SIZE);

    // free memory
    free(matrixA);
    free(matrixB); 
    free(matrixResult);
    free(matrixResultARM);
    
  }

  return 0 ;
}
