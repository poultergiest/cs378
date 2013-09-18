#include <stdio.h>
#include <stdlib.h>
#include <iostream>

/* Feel free to modify the structure (like if you want headers and 
stuff do WHATEVER YOU WANT man. This is just something to start with. 
******************************************************************
***Created and Owned by Ethan Poulter and Jake Wilke*** 
*/

/* Function Declarations */
void naiveMMM(int size);
void fillMatrices(double matrixA[][3], double matrixB[][3], int size);


/* Implementation of the first naive I, J, K matrix multiply */
void naiveMMM(int size) {

  int i,j,k;
  double sum;
  double matrixA [size][3];
  double matrixB [size][3];
  double matrixC [size][size];
 
  fillMatrices(matrixA, matrixB, size); 

   for (i = 0; i < size; i++){
        for (j = 0; j < size; j++){
	    sum = 0;
            for (k = 0; k < size; k++){
                sum += matrixA[i][k] * matrixB[k][j];
            }
	matrixC[i][j] = sum;
        }
   }
	
   std::cout<<"The result is "<<std::endl;
   for(i = 0; i < size; i++){
       for( j = 0; j < size; j++){
	  std:: cout<<matrixC[i][j]<<" ";
	}
   std:: cout<<std::endl;  
   }
}
void fillMatrices(double matrixA[][3], double matrixB[][3], int size) {
   
   int i, j, k;
   double r1, r2;
   
   std::cout<<"Matrix one"<<std::endl;
   for (i = 0; i < size; i++) {
       for(j = 0; j < size; j++) {
          r1 = rand() % 500 + 300;
	  r1 /= rand() % 300 + 1;
          matrixA[i][j] = r1;
	  std::cout<<r1<<" ";;
        }
    std::cout<<std::endl;
    }
    
    std::cout<<"Matrix two"<<std::endl;
    for (i = 0; i < size; i++) {
       for(j = 0; j < size; j++) {
          r2 = rand() % 500 + 300;
	  r2 /= rand() % 300 + 1;
          matrixB[i][j] = r2;
	  std::cout<<r2<<" ";
        }
    std:: cout<<std::endl;
    }
}
int main () {
  naiveMMM(3);

}
