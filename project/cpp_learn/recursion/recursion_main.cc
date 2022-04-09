#include <cassert>   //assert
#include <exception> //static_assert
#include <iostream>
#include <iterator>
#include <memory>
#include <string>

#include "base_context.h"
#include "common_log.h"
#include "log_printf.h"

static int BasicRecursionTest(int n) {

  int answer = 0;
  if (n == 1)
    return 1;
  answer = BasicRecursionTest(n - 1) * n;

  return answer;
}

int main(int argc, char *argv[]) {

  try {
    int base = 3;
    int ret = BasicRecursionTest(base);
    PRINT("%d! = %d", base, ret);
  } catch (std::exception &err) {
    std::cout << "exception:" << err.what() << std::endl;
  } catch (...) {
    std::cout << "exception but do nothing" << std::endl; // do nothing
    throw;                                                //重新抛出
  }
  return 0;
}
