/* compile with g++ -O3 SHA.cpp  -lcrypto -lrt */

#include "openssl/sha.h"
#include <string.h>
#include <stdio.h>
#include <time.h>


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
/*void sha256(unsigned char output[SHA256_DIGEST_LENGTH], const char* input, int len) {
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

void sha256()


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
        //sha256(buffer, src, 128);
        //    printf("%s\n", buffer);
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