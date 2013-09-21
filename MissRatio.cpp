#include <stdio.h>
#include <stdlib.h>
#include <iostream>
//#include <papi.h>

/* Feel free to modify the structure (like if you want headers and 
stuff do WHATEVER YOU WANT man. This is just something to start with. 
******************************************************************
***Created and Owned by Ethan Poulter and Jake Wilke*** 
*/

#define SIZE 3

/* Function Declarations */
void IJK(double matrixA[SIZE][SIZE], double matrixB[SIZE][SIZE], int n);
void IKJ();
void JIK();
void KIJ();
void KJI();
void fillMatrices(double matrixA[SIZE][SIZE], double matrixB[SIZE][SIZE]);

using namespace std;

void printMatrix(double matrix[SIZE][SIZE]) {
  for(int i = 0; i < SIZE; i++){
    for(int j = 0; j < SIZE; j++){
       cout << matrix[i][j] << " ";
    }
     cout << endl;  
  }
}


/* Implementation of the first naive I, J, K matrix multiply */
void IJK(double matrixA[SIZE][SIZE], double matrixB[SIZE][SIZE], int n) {

  int i,j,k;
  //double matrixA [SIZE][SIZE];
  //double matrixB [SIZE][SIZE];
  double matrixC [SIZE][SIZE];

  fillMatrices(matrixA, matrixB); 

  for (i = 0; i < SIZE; i++){
    for (j = 0; j < SIZE; j++){
      for (k = 0; k < SIZE; k++){
        matrixC[i][j] += matrixA[i][k] * matrixB[k][j];
      }
    }
  }

  cout << "The result is " << endl;
  printMatrix(matrixC);
}

void IKJ() {

  int i,j,k;
  double matrixA [SIZE][SIZE];
  double matrixB [SIZE][SIZE];
  double matrixC [SIZE][SIZE];

  fillMatrices(matrixA, matrixB); 

  for (i = 0; i < SIZE; i++){
    for (k = 0; k < SIZE; k++){
      for (j = 0; j < SIZE; j++){
        matrixC[i][j] += matrixA[i][k] * matrixB[k][j];
      }
    }
  }

  cout << "The result is " << endl;
  printMatrix(matrixC);
}

void JIK() {

  int i,j,k;
  double matrixA [SIZE][SIZE];
  double matrixB [SIZE][SIZE];
  double matrixC [SIZE][SIZE];

  fillMatrices(matrixA, matrixB); 

  for (j = 0; j < SIZE; j++){
    for (i = 0; i < SIZE; i++){
      for (k = 0; k < SIZE; k++){
        matrixC[i][j] += matrixA[i][k] * matrixB[k][j];
      }
    }
  }

  cout << "The result is " << endl;
  printMatrix(matrixC);
}

void JKI() {

  int i,j,k;
  double matrixA [SIZE][SIZE];
  double matrixB [SIZE][SIZE];
  double matrixC [SIZE][SIZE];

  fillMatrices(matrixA, matrixB); 

  for (j = 0; j < SIZE; j++){
    for (k = 0; k < SIZE; k++){
      for (i = 0; i < SIZE; i++){
        matrixC[i][j] += matrixA[i][k] * matrixB[k][j];
      }
    }
  }

  cout << "The result is " << endl;
  printMatrix(matrixC);
}

void KIJ() {

  int i,j,k;
  double matrixA [SIZE][SIZE];
  double matrixB [SIZE][SIZE];
  double matrixC [SIZE][SIZE];

  fillMatrices(matrixA, matrixB); 

  for (k = 0; k < SIZE; k++){
    for (i = 0; i < SIZE; i++){
      for (j = 0; j < SIZE; j++){
        matrixC[i][j] += matrixA[i][k] * matrixB[k][j];
      }
    }
  }

  cout << "The result is " << endl;
  printMatrix(matrixC);
}

void KJI() {

  int i,j,k;
  double matrixA [SIZE][SIZE];
  double matrixB [SIZE][SIZE];
  double matrixC [SIZE][SIZE];

  fillMatrices(matrixA, matrixB); 

  for (k = 0; k < SIZE; k++){
    for (j = 0; j < SIZE; j++){
      for (i = 0; i < SIZE; i++){
        matrixC[i][j] += matrixA[i][k] * matrixB[k][j];
      }
    }
  }

  cout << "The result is " << endl;
  printMatrix(matrixC);
}

/* Fills matrices A and B with random doubles by taking random ints 100<x<600 divided by
random ints where 1<x<100 */
void fillMatrices(double matrixA[SIZE][SIZE], double matrixB[SIZE][SIZE]) {
  double r1, r2;

  
  for (int i = 0; i < SIZE; i++) {
    for(int j = 0; j < SIZE; j++) {
      r1  = rand() % 2 + 1;
      //r1 /= rand() % 100 + 1;
      r2  = rand() % 2 + 1;
      //r2 /= rand() % 100 + 1;
      matrixA[i][j] = r1;
      matrixB[i][j] = r2;
    }
  }

  cout << "Matrix one" << endl;
  printMatrix(matrixA);
  cout << "Matrix two" << endl;
  printMatrix(matrixB);
}

int main() {
  double matrix1[SIZE][SIZE];
  double matrix2[SIZE][SIZE];
  cout << "IJK" << endl;
  IJK(matrix1, matrix2, SIZE);
}
