#include <stdio.h>
#include <stdlib.h>


int gcd(int a, int b) {
  int t = 0;
  while((t = a % b) != 0) {
    a = b;
    b = t;
  }
  return b;
}

int ggt(int a, int b) {
  return b ? ggt(b, a % b) : a;
}

int main(int argc, char **argv) {
  printf("%d\n", ggt(atoi(argv[1]), atoi(argv[2])));
  exit(0);
}
