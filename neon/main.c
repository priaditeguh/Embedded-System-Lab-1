#include "matMult_NEON.h"

int main(int argc, char *argv[])
{
  int i, j;

  Timer totalTime;
  initTimer(&totalTime, "Total Time");
	
	if(argc < 2){
		printf("Usage: %s matrix/vector_size\n", argv[0]);
		return 0;
	}
	
	int SIZE = atoi(argv[1]);
	if(SIZE%8 != 0){
		printf("This version implements for ""size = 8*n"" only\n");
		return 0;
	}	else if (SIZE > 128) {
		printf("This version implements for ""size <= 128"" only\n");
		return 0; 
  }
	
	int16_t *mat1 = (int16_t *)memalign(sizeof(int16_t)*8, sizeof(int16_t)*SIZE*SIZE);
		if(mat1==NULL){
		printf("can't allocate the required memory for matrix\n");
		return 0;
	} 

	int16_t *mat2 = (int16_t *)memalign(sizeof(int16_t)*8, sizeof(int16_t)*SIZE*SIZE);
	if(mat2==NULL){
		printf("can't allocate the required memory for matrix\n");
		free(mat1);
		return 0;
	}

	int16_t *prod = (int16_t *)memalign(sizeof(int16_t)*8, sizeof(int16_t)*SIZE*SIZE);
	if(prod==NULL){
		printf("can't allocate the required memory for result_pl\n");
		free(mat1);
		free(mat2);
		free(prod);
		return 0;
	}
	
  int16x8_t *prod2 = (int16x8_t*) malloc (SIZE*SIZE/8 * sizeof(int16x8_t) );
	if(prod2==NULL){
		printf("can't allocate the required memory for result_pl\n");
		free(mat1);
		free(mat2);
		free(prod);
    free(prod2);
		return 0;
	}
 
	int16_t *prod3 = (int16_t *)memalign(sizeof(int16_t)*8, sizeof(int16_t)*SIZE*SIZE);
	if(prod==NULL){
		printf("can't allocate the required memory for result_pl\n");
		free(mat1);
		free(mat2);
		free(prod);
		free(prod);
    free(prod2);
    free(prod3);
		return 0;
	} 
 
//  initMatrix(SIZE,prod2); 
 
	generate_matrix(SIZE,mat1,mat2);
	
	startTimer(&totalTime);
	matMult(SIZE,mat1,mat2,prod);
	stopTimer(&totalTime);
	printTimer(&totalTime);	

	// printResults(SIZE,prod);
 
	startTimer(&totalTime);
	matMult_NEON(SIZE,mat1,mat2,prod3);
	stopTimer(&totalTime);
	printTimer(&totalTime);	
 
	compareResults(SIZE,prod,prod3);
  // printResults(SIZE,prod3);

//  int16x8_t line_one, line_two, result;
//	line_one = vmovq_n_s16(1);  
//	line_two = vld1q_s16(&prod[0]); 
//	result = vmulq_s16(line_one,line_two);	
//   
//  printf("%d\n",vgetq_lane_s16(line_one,0));
//  printf("%d\n",vgetq_lane_s16(line_two,0));
//  printf("%d\n",vgetq_lane_s16(result,0));
 
//	copyNEON(SIZE,prod,prod2);
// 
//  void printResults_NEON(SIZE,prod2)
//	
//  moveNEONtoArm(SIZE,prod2,mat2);
//  printResults(SIZE,mat2);
 
	free(mat1);
	free(mat2);
	free(prod);
  free(prod2);
	free(prod3);
	return 0;
}
