//use "module swap intel gcc" to load the best gcc on lonestar
//Compile (vectorized) with g++ test.cpp -I ~lenharth/eigen/Eigen/ -O3 -ffast-math -march=native
//Compile (reference)  with g++ test.cpp -I ~lenharth/eigen/Eigen/ -O3 -ffast-math -march=native -DEIGEN_DONT_VECTORIZE=1

#include <iostream>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>

using namespace std;

static const int __sz = 3001;
static const int rep = 2;

double deltaTime(timeval& t1, timeval& t2) {
  timeval ret;
  timersub(&t2, &t1, &ret);
  return (double)ret.tv_sec + (double)ret.tv_usec / (double)1000000;
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

void fillMatrix(double** matrix, int n) {
  double r1;

  for (int i = 0; i < n; i++) {
    for(int j = 0; j < n; j++) {
      r1 = static_cast<double>(rand()) / (RAND_MAX+1);
      r1 *= 1000;
      matrix[i][j] = r1;
    }
  }
}

void do_nothing(int n) {
  int sum = 0;
  for(int j = 1; j < n; j++) 
    if(n%j == 0) sum++;
  if(sum > 1) cout << "Cache Flushed." << endl;
}

void FlushCache() {
  int sizeOfL2InBytes = 262144; // size of l2 is 256kb = 262144 bytes
  int sizeOfL2InDoubles = sizeOfL2InBytes / sizeof(double);
  int n = (int) (sqrt(sizeOfL2InDoubles) + 1);  // should be bigger than the square root of the size of doubles to overfill l2 cache
  double** cacheSizedMatrix = AllocateMatrix(n);
  int sum = 0;
  int bsum = 0;
  for(int i = 0; i < n; i++) {
    for(int j = 0; j < n; j++) {
      cacheSizedMatrix[i][j] = rand() % 5;
      sum += cacheSizedMatrix[i][j];
    }
    bsum += sum;
    sum = 0;
  }
  bsum /= n;
  do_nothing(bsum);
  DeallocateMatrix(cacheSizedMatrix, n);
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

void FastMatrixMatrixMultiply(double*** matrix1, double*** matrix2, double*** matrix3, int _sz) {
  int i,j,k;
  int L1_BLOCK_SIZE = 32;
  int L2_BLOCK_SIZE = 256;
  int UNROLL = 8;
  int unrolled_loops = _sz / UNROLL;
  int cleanups       = _sz % UNROLL;
  if(_sz >= 1000) {
    for (int b2k = 0; b2k < _sz; b2k += L2_BLOCK_SIZE) {
      for(int b2i = 0; b2i < _sz; b2i += L2_BLOCK_SIZE) {
        for (int b1k = b2k; b1k < min(b2k+L2_BLOCK_SIZE-1,_sz); b1k += L1_BLOCK_SIZE) {
          for(int b1i = b2i; b1i < min(b2i+L2_BLOCK_SIZE-1,_sz); b1i += L1_BLOCK_SIZE) {
            for(i = b1i; i < min(b1i+L1_BLOCK_SIZE-1,_sz); i++) {
              for (k = b1k; k < min(b1k+L1_BLOCK_SIZE-1,_sz); k++) {
                int temp = (*matrix1)[i][k];
                for (j = 0; j < unrolled_loops; j++) {
                  (*matrix3)[i][j]   += temp * (*matrix2)[k][j];
                  (*matrix3)[i][j+1] += temp * (*matrix2)[k][j+1];
                  (*matrix3)[i][j+2] += temp * (*matrix2)[k][j+2];
                  (*matrix3)[i][j+3] += temp * (*matrix2)[k][j+3];
                  (*matrix3)[i][j+4] += temp * (*matrix2)[k][j+4];
                  (*matrix3)[i][j+5] += temp * (*matrix2)[k][j+5];
                  (*matrix3)[i][j+6] += temp * (*matrix2)[k][j+6];
                  (*matrix3)[i][j+7] += temp * (*matrix2)[k][j+7];
                }
                for (j = 0; j < cleanups; j++) {
                  (*matrix3)[i][j]   += temp * (*matrix2)[k][j];
                }
              }
            }
          }
        }
      }
    }
  } else {
    for(i = 0; i < _sz; i++) {
      for (k = 0; k < _sz; k++) {
        int temp = (*matrix1)[i][k];
        for (j = 0; j < unrolled_loops; j++) {
          (*matrix3)[i][j]   += temp * (*matrix2)[k][j];
          (*matrix3)[i][j+1] += temp * (*matrix2)[k][j+1];
          (*matrix3)[i][j+2] += temp * (*matrix2)[k][j+2];
          (*matrix3)[i][j+3] += temp * (*matrix2)[k][j+3];
          (*matrix3)[i][j+4] += temp * (*matrix2)[k][j+4];
          (*matrix3)[i][j+5] += temp * (*matrix2)[k][j+5];
          (*matrix3)[i][j+6] += temp * (*matrix2)[k][j+6];
          (*matrix3)[i][j+7] += temp * (*matrix2)[k][j+7];
        }
        for (j = 0; j < cleanups; j++) {
          (*matrix3)[i][j]   += temp * (*matrix2)[k][j];
        }
      }
    }
  }
}

int main(int argc, char** argv) {
  double** matrix1;
  double** matrix2;
  double** matrix3;
  srand(getpid());

  for (int _sz = 4; _sz < __sz+1; _sz += (rand() % (_sz*2))) {
    cout << "Initializing matrices of size " << _sz << endl;;
    AllocateTheThreeMatrices(&matrix1, &matrix2, &matrix3, _sz);
    fillMatrix(matrix1, _sz);
    fillMatrix(matrix2, _sz);    
    fillMatrix(matrix3, _sz);
    cout << "Flushing the cache..." << endl;

    double time = 0; 
    FlushCache();
    cout << "Computing..." << endl;
    for (int rep_cnt = 0; rep_cnt < rep; ++rep_cnt) {
      timeval t1, t2;
      gettimeofday(&t1, 0);
      FastMatrixMatrixMultiply(&matrix1, &matrix2, &matrix3, _sz);
      gettimeofday(&t2, 0);
      time += deltaTime(t1,t2);
    }
    time /= rep;
    cout << time << " sec " << ((double)_sz * _sz * _sz * 2) / (1000000000UL * time) << " GFLOPS\n";


    cout << "Deallocating Matrices of size " << _sz << endl;;
    DeallocateTheThreeMatrices(matrix1, matrix2, matrix3, _sz);
  }

  cout << "done\n";

  return 0;
}