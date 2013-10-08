#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <fstream>
#include <sstream>



/* Function Declarations for Part 1 */

void regularMMM(double** matrixA, double** matrixB, double** matrixC, int n);
void vectorizedMMM(double** matrixA, double** matrixB, double** matrixC, int n);
void printMatrix(double** matrix, int n);

using namespace std;

// Prints a square matrix of size n
void printMatrix(double** matrix, int n) {
for(int i = 0; i < n; i++){
  for(int j = 0; j < n; j++){
   cout << matrix[i][j] << " ";
  }
 cout << endl;  
 }
}

// Regular MMM in IJK format
void regularMMM(double** matrixA, double** matrixB, double** matrixC, int n) {
  int i,j,k;

  for (i = 0; i < n; i++){
    for (j = 0; j < n; j++){
      for (k = 0; k < n; k++){
        matrixC[i][j] += matrixA[i][k] * matrixB[k][j];
      }
    }
  }
}

// Vectorized MMM in IJK format *Format subject to change if necessary*
void vectorizedMMM(double** matrixA, double** matrixB, double** matrixC, int n) {
  int i,j,k;

  for (i = 0; i < n; i++){
    for (j = 0; j < n; j++){
      for (k = 0; k < n; k++){
        matrixC[i][j] += matrixA[i][k] * matrixB[k][j];
      }
    }
  }
}

int main(int argc, char const *argv[])
{
  	double** matrix1;
  	double** matrix2;
  	double** matrix3;
	/* code */
	return 0;
}