#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <papi.h>
#include <fstream>

/* Feel free to modify the structure (like if you want headers and 
stuff do WHATEVER YOU WANT man. This is just something to start with. 
******************************************************************
***Created and Owned by Ethan Poulter and Jake Wilke*** 
*/

/* Function Declarations */
void IJK(double** matrixA, double** matrixB, double** matrixC, int n);
void IKJ(double** matrixA, double** matrixB, double** matrixC, int n);
void JIK(double** matrixA, double** matrixB, double** matrixC, int n);
void JKI(double** matrixA, double** matrixB, double** matrixC, int n);
void KIJ(double** matrixA, double** matrixB, double** matrixC, int n);
void KJI(double** matrixA, double** matrixB, double** matrixC, int n);
double** AllocateMatrix(int n);
void DeallocateMatrix(double** matrix, int n);
void fillMatrix(double** matrix, int n);
void printMatrix(double** matrix, int n);

using namespace std;

#define PRINT_ON 0

/* Utility Function */
void initPapi() {
  int retval = PAPI_library_init(PAPI_VER_CURRENT); 
  if (retval != PAPI_VER_CURRENT) {
    fprintf(stderr, "PAPI library init error!\n");
    exit(1);
  }    
}


void addAndStartCounterForInstruction(int* eventSet, int instruction) {
  /* Create the eventset */
  if (PAPI_create_eventset(eventSet) != PAPI_OK) {
    fprintf(stderr, "PAPI_create_eventset error!\n");
    exit(1);
  }
  /* Add instructions to our eventset */
  if (PAPI_add_event(*eventSet, instruction) != PAPI_OK){
    fprintf(stderr, "PAPI_add_event error!\n");
    exit(1);
  }
  /* Start counting events in the eventset */
  if (PAPI_start(*eventSet) != PAPI_OK){
    fprintf(stderr, "PAPI_start error!\n");
    exit(1);
  }
}

void readCounter(int* eventSet, long_long* value) {
  /* Read the counting events in the eventset */
  if (PAPI_read(*eventSet, value) != PAPI_OK) {
    fprintf(stderr, "PAPI_read error!\n");
    exit(1);
  }

  //cout << "Total Load Store Instructions " << (value)[0] << endl;
}

// Prints a square matrix of size n
void printMatrix(double** matrix, int n) {
 /* for(int i = 0; i < n; i++){
    for(int j = 0; j < n; j++){
     cout << matrix[i][j] << " ";
   }
   cout << endl;  
 }
 */
}

// Fills a matrix of size n with random doubles
void fillMatrix(double** matrix, int n) {
  double r1;

  for (int i = 0; i < n; i++) {
    for(int j = 0; j < n; j++) {
      r1  = (double)rand() / (RAND_MAX+1);
      r1 *= 1000;
      matrix[i][j] = r1;
    }
  }

 // cout << "Matrix: " << endl;
 // printMatrix(matrix, n);
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
  int sizeOfL2InBytes = 2097152*5; // this is just a guess, approx 2mb * 5
  int sizeOfL2InDoubles = sizeOfL2InBytes / sizeof(double);
  int n = (int) (sqrt(sizeOfL2InDoubles) + 1);  // should be bigger than the square root of the size of doubles to overfill l2 cache
  double** cacheSizedMatrix = AllocateMatrix(n);
  int sum = 0;
  for(int i = 0; i < n; i++) {
    for(int j = 0; j < n; j++) {
      cacheSizedMatrix[i][j] = rand() % 10;
      sum += cacheSizedMatrix[i][j];
    }
    //cout << sum << endl;
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

void WriteToFile(string name, long_long num) {
  ofstream file;
  file.open(name.c_str(), ios::out | ios::app);
  file << num << ", ";
  file.close();
}

int main() {
  double** matrix1;
  double** matrix2;
  double** matrix3;
  int EventSet=PAPI_NULL;
  long_long values[1];
  int instruction = PAPI_L2_TCM;
  initPapi(); 

  addAndStartCounterForInstruction(&EventSet, instruction); 
  string filename = "";
  string instruction_name = "PAPI_L2_TCM.data";

  ofstream file;
  file.open(instruction_name.c_str(), ios::out | ios::app);

  for(int i = 1; i < 501; i++) {
    //cout << "Matrix of size " << i << endl;
    int n = i;
 
    AllocateTheThreeMatrices(&matrix1, &matrix2, &matrix3, n);

    fillMatrix(matrix1, n);
    fillMatrix(matrix2, n);
    fillMatrix(matrix3, n);
    FlushCache();

    /* start measuring */
    PAPI_reset(EventSet);  
    //cout << "IJK" << endl;
    IJK(matrix1, matrix2, matrix3, n);
    readCounter(&EventSet, values);
    /* stop measuring then print values */

    filename = "IJK";
    //WriteToFile(filename+instruction_name, values[0]);
    file << filename.c_str() << endl << values[0] << endl;

    //DeallocateTheThreeMatrices(matrix1, matrix2, matrix3, n);

    // ----------------------------------------

    AllocateTheThreeMatrices(&matrix1, &matrix2, &matrix3, n);

    fillMatrix(matrix1, n);
    fillMatrix(matrix2, n);
    fillMatrix(matrix3,	n);
    FlushCache();     

    /* start measuring */
    PAPI_reset(EventSet);  
    //cout << "IKJ" << endl;
    IKJ(matrix1, matrix2, matrix3, n);
    readCounter(&EventSet, values);
    /* stop measuring */

    filename = "IKJ";
    //WriteToFile(filename+instruction_name, values[0]);
    file << filename.c_str() << endl << values[0] << endl;
    //DeallocateTheThreeMatrices(matrix1, matrix2, matrix3, n);

    // ----------------------------------------

    //AllocateTheThreeMatrices(&matrix1, &matrix2, &matrix3, n);

    fillMatrix(matrix1, n);
    fillMatrix(matrix2, n);
    fillMatrix(matrix3,	n);
    FlushCache();

    /* start measuring */
    PAPI_reset(EventSet);  
    //cout << "JIK" << endl;
    JIK(matrix1, matrix2, matrix3, n);
    readCounter(&EventSet, values);
    /* stop measuring */

    filename = "JIK";
//    WriteToFile(filename+instruction_name, values[0]);
    file << filename << endl << values[0] << endl;
    //DeallocateTheThreeMatrices(matrix1, matrix2, matrix3, n);

    // ----------------------------------------

    //AllocateTheThreeMatrices(&matrix1, &matrix2, &matrix3, n);

    fillMatrix(matrix1, n);
    fillMatrix(matrix2, n);
    fillMatrix(matrix3,	n);
    FlushCache();    

    /* start measuring */
    PAPI_reset(EventSet);  
    //cout << "JKI" << endl;
    JKI(matrix1, matrix2, matrix3, n);
    readCounter(&EventSet, values);
    /* stop measuring */

    filename = "JKI";
//    WriteToFile(filename+instruction_name, values[0]);
    file << filename.c_str() << endl << values[0] << endl;
    //DeallocateTheThreeMatrices(matrix1, matrix2, matrix3, n);

    // ----------------------------------------

    //AllocateTheThreeMatrices(&matrix1, &matrix2, &matrix3, n);

    fillMatrix(matrix1, n);
    fillMatrix(matrix2, n);
    fillMatrix(matrix3,	n);
    FlushCache();    

    /* start measuring */
    PAPI_reset(EventSet);  
    //cout << "KIJ" << endl;
    KIJ(matrix1, matrix2, matrix3, n);
    readCounter(&EventSet, values);
    /* stop measuring */

    filename = "KIJ";
    //WriteToFile(filename+instruction_name, values[0]);
file << filename.c_str() << endl << values[0] << endl;
    //DeallocateTheThreeMatrices(matrix1, matrix2, matrix3, n);

    // ----------------------------------------

    //AllocateTheThreeMatrices(&matrix1, &matrix2, &matrix3, n);

    fillMatrix(matrix1, n);
    fillMatrix(matrix2, n);
    fillMatrix(matrix3,	n);
    FlushCache();    

    /* start measuring */
    PAPI_reset(EventSet);  
    //cout << "KJI" << endl;
    KJI(matrix1, matrix2, matrix3, n);
    readCounter(&EventSet, values);
    /* stop measuring */

    filename = "KJI";
//    WriteToFile(filename+instruction_name, values[0]);
    file << filename.c_str() << endl << values[0] << endl;
    DeallocateTheThreeMatrices(matrix1, matrix2, matrix3, n);

  }
   file.close();
   PAPI_stop(EventSet, values);
}

