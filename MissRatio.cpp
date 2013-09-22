#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
//#include <papi.h>

/* Feel free to modify the structure (like if you want headers and 
stuff do WHATEVER YOU WANT man. This is just something to start with. 
******************************************************************
***Created and Owned by Ethan Poulter and Jake Wilke*** 
*/

/* Function Declarations */
void IJK(double** matrixA, double** matrixB, double** matrixC, int n);
void IKJ(double** matrixA, double** matrixB, double** matrixC, int n);
void JIK(double** matrixA, double** matrixB, double** matrixC, int n);
void KIJ(double** matrixA, double** matrixB, double** matrixC, int n);
void KJI(double** matrixA, double** matrixB, double** matrixC, int n);
double** AllocateMatrix(int n);
void DeallocateMatrix(double** matrix, int n);
void fillMatrix(double** matrix, int n);
void printMatrix(double** matrix, int n);

using namespace std;

#define PRINT_ON 0

/* Utility Function */

// Prints a square matrix of size n
void printMatrix(double** matrix, int n) {
  for(int i = 0; i < n; i++){
    for(int j = 0; j < n; j++){
     cout << matrix[i][j] << " ";
   }
   cout << endl;  
 }
}

// Fills a matrix of size n with random doubles
void fillMatrix(double** matrix, int n) {
  double r1;

  for (int i = 0; i < n; i++) {
    for(int j = 0; j < n; j++) {
      r1  = rand() % 3 + 1;
      //r1 /= rand() % 100 + 1;
      matrix[i][j] = r1;
    }
  }

  cout << "Matrix: " << endl;
  printMatrix(matrix, n);
}


// Creates a square matrix of size n and fills with 0s
double** AllocateMatrix(int n) {
  double** matrix = new double*[n];
  for(int i = 0; i < n; i++) {
    matrix[i] = new double[n];
    for(int j = 0; j < n; j++) {
      matrix[i][j] = 0;
    }
  }
  return matrix;
}

// Destroys a square matrix of size n
void DeallocateMatrix(double** matrix, int n) {
  for(int i = 0; i < n; i++) {
    delete [] matrix[i];
  }
  delete [] matrix;
}

// TODO: Find n such that the square matrix of doubles is bigger than the size of L2 cache
void FlushCache() {
  int sizeOfL2InBytes = 2097152; // this is just a guess, approx 2mb
  int sizeOfL2InDoubles = sizeOfL2InBytes / sizeof(double);
  int n = (int) (sqrt(sizeOfL2InDoubles) + 1);  // should be bigger than the square root of the size of doubles to overfill l2 cache
  double** cacheSizedMatrix = AllocateMatrix(n);
  int sum = 0;
  for(int i = 0; i < n; i++) {
    for(int j = 0; j < n; j++) {
      cacheSizedMatrix[i][j] = rand() % 10;
      sum += cacheSizedMatrix[i][j];
    }
    cout << sum << endl;
    sum = 0;
  }
  DeallocateMatrix(cacheSizedMatrix, n);
}


/* Implementation of the first naive I, J, K matrix multiply */
void IJK(double** matrixA, double** matrixB, double** matrixC, int n) {
  int i,j,k;

  for (i = 0; i < n; i++){
    for (j = 0; j < n; j++){
      for (k = 0; k < n; k++){
        matrixC[i][j] += matrixA[i][k] * matrixB[k][j];
      }
    }
  }

  cout << "The result is " << endl;
  printMatrix(matrixC, n);
}


void IKJ(double** matrixA, double** matrixB, double** matrixC, int n) {
  int i,j,k;

  for (i = 0; i < n; i++){
    for (k = 0; k < n; k++){
      for (j = 0; j < n; j++){
        matrixC[i][j] += matrixA[i][k] * matrixB[k][j];
      }
    }
  }

  cout << "The result is " << endl;
  printMatrix(matrixC, n);
}

void JIK(double** matrixA, double** matrixB, double** matrixC, int n) {
  int i,j,k;

  for (j = 0; j < n; j++){
    for (i = 0; i < n; i++){
      for (k = 0; k < n; k++){
        matrixC[i][j] += matrixA[i][k] * matrixB[k][j];
      }
    }
  }

  cout << "The result is " << endl;
  printMatrix(matrixC, n);
}

void JKI(double** matrixA, double** matrixB, double** matrixC, int n) {
  int i,j,k;

  for (j = 0; j < n; j++){
    for (k = 0; k < n; k++){
      for (i = 0; i < n; i++){
        matrixC[i][j] += matrixA[i][k] * matrixB[k][j];
      }
    }
  }

  cout << "The result is " << endl;
  printMatrix(matrixC, n);
}

void KIJ(double** matrixA, double** matrixB, double** matrixC, int n) {
  int i,j,k;

  for (k = 0; k < n; k++){
    for (i = 0; i < n; i++){
      for (j = 0; j < n; j++){
        matrixC[i][j] += matrixA[i][k] * matrixB[k][j];
      }
    }
  }

  cout << "The result is " << endl;
  printMatrix(matrixC, n);
}

void KJI(double** matrixA, double** matrixB, double** matrixC, int n) {
  int i,j,k;

  for (k = 0; k < n; k++){
    for (j = 0; j < n; j++){
      for (i = 0; i < n; i++){
        matrixC[i][j] += matrixA[i][k] * matrixB[k][j];
      }
    }
  }

  cout << "The result is " << endl;
  printMatrix(matrixC, n);
}

void AllocateTheThreeMatrices(double*** matrixA, double*** matrixB, double*** matrixC, int n) {
  *matrixA = AllocateMatrix(n);
  *matrixB = AllocateMatrix(n);
  *matrixC = AllocateMatrix(n);
}

void DeallocateTheThreeMatrices(double** matrixA, double** matrixB, double** matrixC, int n) {
  DeallocateMatrix(matrixA, n);
  DeallocateMatrix(matrixB, n);
  DeallocateMatrix(matrixC, n);
}

int main() {
  double** matrix1;
  double** matrix2;
  double** matrix3;
  for(int i = 4; i < 5; i++) {
    cout << "Matrix of size " << i << endl;
    int n = i;
    
    AllocateTheThreeMatrices(&matrix1, &matrix2, &matrix3, n);

    fillMatrix(matrix1, n);
    fillMatrix(matrix2, n);

    /* start measuring */
    cout << "IJK" << endl;
    IJK(matrix1, matrix2, matrix3, n);
    /* stop measuring */

    

    // ----------------------------------------

    AllocateTheThreeMatrices(&matrix1, &matrix2, &matrix3, n);

    fillMatrix(matrix1, n);
    fillMatrix(matrix2, n);

    /* start measuring */
    IKJ(matrix1, matrix2, matrix3, n);
    /* stop measuring */

    DeallocateTheThreeMatrices(matrix1, matrix2, matrix3, n);

    // ----------------------------------------

    AllocateTheThreeMatrices(&matrix1, &matrix2, &matrix3, n);

    fillMatrix(matrix1, n);
    fillMatrix(matrix2, n);

    /* start measuring */
    JIK(matrix1, matrix2, matrix3, n);
    /* stop measuring */

    DeallocateTheThreeMatrices(matrix1, matrix2, matrix3, n);

    // ----------------------------------------

    AllocateTheThreeMatrices(&matrix1, &matrix2, &matrix3, n);

    fillMatrix(matrix1, n);
    fillMatrix(matrix2, n);

    /* start measuring */
    JKI(matrix1, matrix2, matrix3, n);
    /* stop measuring */

    DeallocateTheThreeMatrices(matrix1, matrix2, matrix3, n);

    // ----------------------------------------

    AllocateTheThreeMatrices(&matrix1, &matrix2, &matrix3, n);

    fillMatrix(matrix1, n);
    fillMatrix(matrix2, n);

    /* start measuring */
    KIJ(matrix1, matrix2, matrix3, n);
    /* stop measuring */

    DeallocateTheThreeMatrices(matrix1, matrix2, matrix3, n);

    // ----------------------------------------

    AllocateTheThreeMatrices(&matrix1, &matrix2, &matrix3, n);

    fillMatrix(matrix1, n);
    fillMatrix(matrix2, n);

    /* start measuring */
    KJI(matrix1, matrix2, matrix3, n);
    /* stop measuring */

    DeallocateTheThreeMatrices(matrix1, matrix2, matrix3, n);

  }
}

