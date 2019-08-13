/*  ----------------------------------- OS Specific Headers           */
#include <stdio.h>
#include <stdlib.h>
#include "matrixMultiplicationNEON.h"
#include "Timer.h"

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
  
  // Timer totalTime;
  // initTimer(&totalTime, "Total Time");
  long long start;

  if (argc != 3) {
    printf ("Usage : %s <absolute path of DSP executable> "
     "<matrix size>\n", 
     argv [0]) ;
  } else {
    SIZE = atoi(argv[2]); 
    if (SIZE > 128) {
      printf("This version implements for ""size <= 128"" only\n");
      return 0; 
    }

    if (SIZE%DATA_PER_INSTANCE != 0)
    {
      SIZE_ceil = (SIZE/DATA_PER_INSTANCE)*DATA_PER_INSTANCE + DATA_PER_INSTANCE;
    } else {
      SIZE_ceil = SIZE;
    } 
    
    // initialize the matrix
    matrixA = (int32_t*) malloc ( SIZE*SIZE * sizeof(int32_t) );
    if(matrixA==NULL){
      printf("can't allocate the required memory for matrix\n");
      return 0;
    } 

    matrixB = (int32_t*) malloc ( SIZE*SIZE * sizeof(int32_t) );
    if(matrixB==NULL){
      printf("can't allocate the required memory for matrix\n");
      free(matrixB);
      return 0;
    } 

    matrixResultARM = (int32_t*) malloc ( SIZE*SIZE * sizeof(int32_t) );
    if(matrixResultARM==NULL){
      printf("can't allocate the required memory for result_pl\n");
      free(matrixA);
      free(matrixB);
      return 0;
    }

    matrixResult = (int32_t*) malloc ( SIZE*SIZE * sizeof(int32_t) );
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
    start = get_usec();
    matrixMultiplication(SIZE_ceil,matrixA,matrixB,matrixResultARM);
    printf("Execution time %lld us.\n", get_usec()-start);
    
    printf("Implementation Matrix Multiplication\n");
    if (SIZE < 64) {
      // run the pool for computing
      dspExecutable = argv [1] ;

      pool_notify_Main (dspExecutable, SIZE_ceil, matrixA, matrixB, matrixResult);
    }
    else{
      start = get_usec();
      matrixMultiplicationNEON(SIZE_ceil, matrixA, matrixB, matrixResult);
      printf("Execution time %lld us.\n", get_usec()-start);
    }

    #ifdef DEBUG
    printf("SIZE: %d\n",SIZE);
    printf("Matrix A \n");
    printResults(SIZE, matrixA);
    printf("Matrix B \n");
    printResults(SIZE, matrixB);
    printf("RESULT ARM \n");
    printResults(SIZE, matrixResultARM);
    printf("RESULT \n");
    printResults(SIZE, matrixResult);
    #endif
    
    // compare the result
    // compareResults(SIZE, matrixResult, matrixResultARM);

    // free memory
    free(matrixA);
    free(matrixB); 
    free(matrixResult);
    free(matrixResultARM);
    
  }

  return 0 ;
}
