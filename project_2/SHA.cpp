/* compile with g++ -O3 SHA.cpp  -lcrypto -lrt */

#include "openssl/sha.h"
#include <string.h>
#include <stdio.h>
#include <time.h>

using namespace std;

/*struct block {
      char a[60];
      uint32_t nonce;
      char b[64];
};*/

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

/*void dumpHash(unsigned char hash[SHA256_DIGEST_LENGTH]) {
  int i = 0;
  for(i = 0; i < SHA256_DIGEST_LENGTH; i++) {
    printf("%02x", hash[i]);
  }
}*/

/* Professor Implementation of SHA */
/*void prof_sha256(unsigned char output[SHA256_DIGEST_LENGTH], const char* input, int len) {
    unsigned char hash1[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256,sha256_pass2;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, input, len);
    SHA256_Final(hash1, &sha256);

     // printf("first pass: ");
     // dumpHash(hash1);
     // printf("\n");

    SHA256_Init(&sha256_pass2);
    SHA256_Update(&sha256_pass2, hash1, SHA256_DIGEST_LENGTH);
    SHA256_Final(output, &sha256_pass2);

     // printf("second pass: ");
     // dumpHash(output);
     // printf("\n");
}*/


void xsha256(unsigned char output[SHA256_DIGEST_LENGTH], const char* input, int len){

  char* message = new char [len];
  //strncpy(input, message, len);


  unsigned int h0, h1, h2, h3, h4, h5, h6, h7, h8, a, b, c, d, e, f, g, h;
  h0 = 0x6a09e667;
  h1 = 0xbb67ae85;
  h2 = 0x3c6ef372;
  h3 = 0xa54ff53a;
  h4 = 0x510e527f;
  h5 = 0x9b05688c;
  h6 = 0x1f83d9ab;
  h7 = 0x5be0cd19;

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

  //Pre-Processing

  int length_before_pre_processing = strlen(message) * 8;

  message += '1';

  while(strlen(input)*sizeof(char)*8 % 512 != 448) {
    message += '0';
  }

  //Process the message into 512-bit chunks
  for (int i = 0; i <= strlen(input)*sizeof(char)*8; i+= 512) {
    //create chunk;
  }

  for(/*each chunk*/int i = 0; i < 1; i++) {
    //copy chunk into first 16 words of the message schedule array w[0..15]

    for(int i = 16; i <= 63; i++) {


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

    }
  }

}


int main() {
  unsigned char buffer[SHA256_DIGEST_LENGTH];
  char src[128];
  printf("hash size: %d\n", sizeof(buffer));
  memset(src, 0, 128);
  
  timespec start,end,res;
  clock_gettime(CLOCK_MONOTONIC, &start);
  for (int k = 0; k < 256; ++k) {
    for (int j = 0; j < 256; ++j) {
      for (int i = 0; i < 256; ++i) {
        src[60] = i; 
        src[61] = j;
        src[62] = k;
        //printf("testing %d, %d, %d\n", i, j, k);
        //prof_sha256(buffer, src, 128);
        //printf("%s\n", buffer);
      }
    }
  }
  clock_gettime(CLOCK_MONOTONIC, &end);
  res = diff(start,end);
  printf("Time: %d %d\n", res.tv_sec, res.tv_nsec);
  double t = res.tv_sec * 1000 + res.tv_nsec / 1000000;
  printf("MHashes/sec: %f\n", (double)256*256*256/(t*1000));
  return 0;
}