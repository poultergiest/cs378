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

static const int __sz = 2049;
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
      r1 = (double)rand() / (RAND_MAX+1);
      r1 *= 1000;
      matrix[i][j] = r1;
    }
  }
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

int main(int argc, char** argv) {
  double** matrix1;
  double** matrix2;
  double** matrix3;
  int i,j,k;

  srand(getpid());

  for (int _sz = 2048; _sz < __sz; _sz += (rand() % (_sz*2))) {
   
    AllocateTheThreeMatrices(&matrix1, &matrix2, &matrix3, _sz);
    fillMatrix(matrix1, _sz);
    fillMatrix(matrix2, _sz);    
    std::cout << "Initializing matrices of size " << _sz << "\n";
    
    double time = 0;
    

    for (int i = 0; i < rep; ++i) {
      timeval t1, t2;
      gettimeofday(&t1, 0);
      for (i = 0; i < _sz; i++) {
    	for (k = 0; k < _sz; k++) {
      		for (j = 0; j < _sz; j++) {
        		matrix3[i][j] += matrix1[i][k] * matrix2[k][j];
      		}	
    	} 
  	}
      gettimeofday(&t2, 0);
      time += deltaTime(t1,t2);
    }
    time /= 3;
    std::cout << time << " sec " << ((double)_sz * _sz * _sz * 2) / (1000000000UL * time) << " GFLOPS\n";

    std::cout << "Deallocating Matrices of size " << _sz << "\n";
    DeallocateTheThreeMatrices(matrix1, matrix2, matrix3, _sz);

  }
 
  std::cout << "done\n";

  return 0;
}
