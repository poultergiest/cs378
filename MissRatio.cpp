#include <stdio.h>
#include <stdlib.h>
#include <iostream>

/* Feel free to modify the structure (like if you want headers and 
stuff do WHATEVER YOU WANT man. This is just something to start with. 
******************************************************************
***Created and Owned by Ethan Poulter and Jake Wilke*** 
*/


void naiveMMM(int size) {

  int i,j,k;
  double r1, r2;
  int matrixA [size][size];
  int matrixB [size][size];
  int matrixC [size][size];
  
   std::cout<<"Matrix one"<<std::endl;
   for (i = 0; i < size; i++) {
       for(j = 0; j < size; j++) {
          r1 = rand() % 2 + 1;
          matrixA[i][j] = r1;
	  std::cout<<r1<<" ";;
        }
    std:: cout<<std::endl;
    }
    
    std::cout<<"Matrix two"<<std::endl;
    for (i = 0; i < size; i++) {
       for(j = 0; j < size; j++) {
          r2 = rand() % 2 + 1;
          matrixB[i][j] = r2;
	  std::cout<<r2<<" ";;
        }
    std:: cout<<std::endl;
    }
   
   for (i = 0; i < size; i++){
        for (j = 0; j < size; j++){
            for (k = 0; k < size; k++){
                matrixC[i][j] += matrixA[i][k] * matrixB[k][j];
            }
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

int main () {
  naiveMMM(3);

}
