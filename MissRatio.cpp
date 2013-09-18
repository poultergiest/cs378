#include <stdio.h>
#include <stdlib.h>
#include <iostream>

/* Feel free to modify the structure (like if you want headers and 
stuff do WHATEVER YOU WANT man. This is just something to start with. 
******************************************************************
***Created and Owned by Ethan Poulter and Jake Wilke*** 
*/

/* Function Declarations */
void IJK(int size);
void IKJ(int size);
void JIK(int size);
void KIJ(int size);
void KJI(int size);
void fillMatrices(double matrixA[][3], double matrixB[][3], int size);


/* Implementation of the first naive I, J, K matrix multiply */
void IJK(int size) {

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

void IKJ(int size) {

  int i,j,k;
  double sum;
  double matrixA [size][3];
  double matrixB [size][3];
  double matrixC [size][size];
 
  fillMatrices(matrixA, matrixB, size); 

   for (i = 0; i < size; i++){
        for (k = 0; k < size; k++){
	    sum = 0;
            for (j = 0; j < size; j++){
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

void JIK(int size) {

  int i,j,k;
  double sum;
  double matrixA [size][3];
  double matrixB [size][3];
  double matrixC [size][size];
 
  fillMatrices(matrixA, matrixB, size); 

   for (j = 0; j < size; j++){ 
	for (i = 0; i < size; i++){
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

void JKI(int size) {

  int i,j,k;
  double sum;
  double matrixA [size][3];
  double matrixB [size][3];
  double matrixC [size][size];
 
  fillMatrices(matrixA, matrixB, size); 

   for (j = 0; j < size; j++){
        for (k = 0; k < size; k++){
	    sum = 0;
            for (i = 0; i < size; i++){
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

void KIJ(int size) {

  int i,j,k;
  double sum;
  double matrixA [size][3];
  double matrixB [size][3];
  double matrixC [size][size];
 
  fillMatrices(matrixA, matrixB, size); 

   for (k = 0; k < size; k++){
        for (i = 0; i < size; i++){
	    sum = 0;
            for (j = 0; j < size; j++){
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

void KJI(int size) {

  int i,j,k;
  double sum;
  double matrixA [size][3];
  double matrixB [size][3];
  double matrixC [size][size];
 
  fillMatrices(matrixA, matrixB, size); 

   for (k = 0; k < size; k++){
        for (j = 0; j < size; j++){
	    sum = 0;
            for (i = 0; i < size; i++){
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

/* Fills matrices A and B with random doubles by taking random ints 100<x<500 divided by
 random ints where 1<x<100 */
void fillMatrices(double matrixA[][3], double matrixB[][3], int size) {
   
   int i, j, k;
   double r1, r2;
   
   std::cout<<"Matrix one"<<std::endl;
   for (i = 0; i < size; i++) {
       for(j = 0; j < size; j++) {
          r1 = rand() % 500 + 100;
	  r1 /= rand() % 100 + 1;
          matrixA[i][j] = r1;
	  std::cout<<r1<<" ";;
        }
    std::cout<<std::endl;
    }
    
    std::cout<<"Matrix two"<<std::endl;
    for (i = 0; i < size; i++) {
       for(j = 0; j < size; j++) {
          r2 = rand() % 500 + 100;
	  r2 /= rand() % 100 + 1;
          matrixB[i][j] = r2;
	  std::cout<<r2<<" ";
        }
    std:: cout<<std::endl;
    }
}


int main () {
  int size = 3; 

  std::cout<<"IJK"<<std::endl;
  IJK(size);
  std::cout<<"IKJ"<<std::endl;
  IKJ(size);
  std::cout<<"JIK"<<std::endl;
  JIK(size);
  std::cout<<"JKI"<<std::endl;
  JKI(size); 
  std::cout<<"KIJ"<<std::endl;
  KIJ(size);
  std::cout<<"KJI"<<std::endl; 
  KJI(size);
}
