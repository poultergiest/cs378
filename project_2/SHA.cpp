/* compile with g++ -O3 SHA.cpp  -lcrypto -lrt */

#include "openssl/sha.h"
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <time.h>
#include <stdint.h>
#include <cstdlib>
#include "immintrin.h"

using namespace std;

struct block {
      char a[60];
      uint32_t nonce;
      char b[64];
};

timespec diff(timespec start, timespec end);


void dumpHash(unsigned char hash[SHA256_DIGEST_LENGTH]) {
  int i = 0;
  for(i = 0; i < SHA256_DIGEST_LENGTH; i++) {
    printf("%02x", hash[i]);
  }
  cout << endl;
}

/* Professor Implementation of SHA */
void prof_sha256(unsigned char output[SHA256_DIGEST_LENGTH], const char* input, int len) {
    unsigned char hash1[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256,sha256_pass2;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, input, len);
//    SHA256_Final(hash1, &sha256);
    SHA256_Final(output, &sha256);
     // printf("first pass: ");
     // dumpHash(hash1);
     // printf("\n");

//    SHA256_Init(&sha256_pass2);
//    SHA256_Update(&sha256_pass2, hash1, SHA256_DIGEST_LENGTH);
//    SHA256_Final(output, &sha256_pass2);

     // printf("second pass: ");
     //dumpHash(output);
     // printf("\n");
}

unsigned int k [] = {
  0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
  0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
  0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
  0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
  0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
  0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
  0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
  0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
  };


void PrintCharArray(char* c, int len) {
  for(int i = 0; i < len; ++i) {
    cout << (int) c[i] << ", ";
  }
  cout << endl;
}


void xsha256(unsigned char output[SHA256_DIGEST_LENGTH], char* input, int len) {
  int num_chunks = 1;
  if(len == 128) num_chunks = 3;

  char* data = new char [64*num_chunks];
  memset(data, 0, 64*num_chunks);
  unsigned int h0, h1, h2, h3, h4, h5, h6, h7, a, b, c, d, e, f, g, h;
  h0 = 0x6a09e667;
  h1 = 0xbb67ae85;
  h2 = 0x3c6ef372;
  h3 = 0xa54ff53a;
  h4 = 0x510e527f;
  h5 = 0x9b05688c;
  h6 = 0x1f83d9ab;
  h7 = 0x5be0cd19;

  //Pre-Processing
  int length_before_pre_processing_in_bits = len * 8;
  strncpy(data, input, len);
  data[len] = 0x80;
  int index = ((len / 64) + 1) * 64 - 9;
  int* plen = (int*) &(data[index]);
  *plen = length_before_pre_processing_in_bits;

  //Process the message into 512-bit chunks
  char chunks[num_chunks][64];

  for(int c = 0; c < num_chunks; c++) {
    for (int i = 0; i < 64; i+=4) {
      //create chunk;
      chunks[c][i] = data[c*num_chunks+i];
      chunks[c][i+1] = data[c*num_chunks+(i+1)];
      chunks[c][i+2] = data[c*num_chunks+(i+2)];
      chunks[c][i+3] = data[c*num_chunks+(i+3)];
    }
    //cout << "loop: " << c << endl;
    //PrintCharArray(chunks[c], 64);
  }

  for(int chunk = 0; chunk < num_chunks; chunk++) {
    //copy chunk into first 16 words of the message schedule array w[0..15]
    int w[64];
    for(int i = 0; i < 16; i+=4) {
      //unroll to visulaize vectorization
      w[i] = (int) (chunks[chunk][i*4]);
      w[i+1] = (int)(chunks[chunk][(i+1)*4]);
      w[i+2] = (int)(chunks[chunk][(i+2)*4]);
      w[i+3] = (int)(chunks[chunk][(i+3)*4]);
    }

    for(int i = 16; i <= 63; i++) {
      int s0 = _rotr(w[i-15], 7) ^ _rotr(w[i-15], 18) ^ w[i-15] >> 3;
      int s1 = _rotr(w[i-2], 17) ^ _rotr(w[i-2], 19) ^ w[i-2] >> 10;
      w[i] = w[i-16] + s0 + w[i-7] + s1;
    }
    a = h0;
    b = h1;
    c = h2; 
    d = h3; 
    e = h4; 
    f = h5;
    g = h6;
    h = h7;  

    for (int i = 0; i <= 63; i++) {
      int S1 = _rotr(e, 6) ^ _rotr(e, 11) ^ _rotr(e, 25);
      int ch = (e & f) ^ ((~e) & g);
      int temp1 = h + S1 + ch + k[i] + w[i];
      int S0 = _rotr(a, 2) ^ _rotr(a, 13) ^ _rotr(a, 22);
      int maj = (a & b) ^ (a & c) ^ (b & c);
      int temp2 = S0 + maj;

      g = f;
      h = g;
      f = e;
      e = d + temp1;
      d = c;
      c = b;
      b = a;
      a = temp1 + temp2;
    }

    // Add the compressed chunk to the current hash value
    h0 = h0 + a;
    h1 = h1 + b;
    h2 = h2 + c;
    h3 = h3 + d;
    h4 = h4 + e;
    h5 = h5 + f;
    h6 = h6 + g;
    h7 = h7 + h;
  }

  
  int* ptemp = (int*) &(output[0]);
  *ptemp = h0;
  ptemp = (int*) &(output[4]);
  *ptemp = h1;
  ptemp = (int*) &(output[8]);
  *ptemp = h2;
  ptemp = (int*) &(output[12]);
  *ptemp = h3;
  ptemp = (int*) &(output[16]);
  *ptemp = h4;
  ptemp = (int*) &(output[20]);
  *ptemp = h5;
  ptemp = (int*) &(output[24]);
  *ptemp = h6;
  ptemp = (int*) &(output[28]);
  *ptemp = h7;
  delete [] data;
}

void FillRandomBytes(char* dest, int num) {
  for(int i = 0; i < num; ++i) {
    dest[i] = 1; //rand() % 256;
  }
}


void PrintBlock(block b) {
  cout << "Block.a = ";
  PrintCharArray(b.a, 60);
  cout << "Block.nonce = " << b.nonce << endl;
  cout << "Block.b = ";
  PrintCharArray(b.b, 64);
  cout << endl;
}

int main() {
  srand(0);
  unsigned char buffer[SHA256_DIGEST_LENGTH];
  printf("hash size: %zu\n", sizeof(buffer));
  
  timespec start,end,res;
  clock_gettime(CLOCK_MONOTONIC, &start);
  
  for (uint32_t i = 0; i < ~0; ++i) {
    block M;
    FillRandomBytes(M.a, 60);
    M.nonce = i;
    FillRandomBytes(M.b, 64);
    xsha256(buffer, M.a, 128);
    xsha256(buffer, (char*)buffer, SHA256_DIGEST_LENGTH);
    dumpHash(buffer);
    memset(buffer, 0, SHA256_DIGEST_LENGTH);
    prof_sha256(buffer, M.a, 128);
    cout << endl;
    dumpHash(buffer);
  }
  clock_gettime(CLOCK_MONOTONIC, &end);
  res = diff(start,end);
  printf("Time: %zu %zu\n", res.tv_sec, res.tv_nsec);
  double t = res.tv_sec * 1000 + res.tv_nsec / 1000000;
  printf("MHashes/sec: %f\n", (double)256*256*256/(t*1000));
  return 0;
}


timespec diff(timespec start, timespec end)
{
  timespec temp;
  if ((end.tv_nsec-start.tv_nsec)<0) {
    temp.tv_sec = end.tv_sec-start.tv_sec-1;
    temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
  } else {
    temp.tv_sec = end.tv_sec-start.tv_sec;
    temp.tv_nsec = end.tv_nsec-start.tv_nsec;
  }
  return temp;
}
