#include <stdio.h>
extern "C" {
#include "add1.h"
}
int main()

{
  printf("a+b = %d\n", add(1, 2));
  return 0;
}
