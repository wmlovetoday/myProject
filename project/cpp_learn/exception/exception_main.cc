#include <cassert>  //assert
#include <cstdint>
#include <exception>  //static_assert
#include <iostream>
#include <string>

#include "base_context.h"
#include "common_log.h"
#include "log_printf.h"

static int DoTask() { throw std::runtime_error{"log category failed"}; }

static void ExceptionTest(int32_t *ptr0, int32_t *ptr1) {
  if (ptr0 == nullptr) {
    PRINT("excetion 1");
    throw(10);
  }
  if (ptr1 == nullptr) {
    PRINT("excetion 2");
    std::string ex{"prt1 is null"};
    throw std::invalid_argument{ex};
    // throw std::invalid_argument{"prt1 is null"};
  }
  throw std::runtime_error{"do nothing"};
}

int main(int argc, char *argv[]) {
  base_con::PrintFlag("exception test");

  PTS_INFO("i am zlog !!!");
  PRINT("i am C log !!!");

  try {
    // DoTask();
    int p;
    ExceptionTest(&p, 0);
  } catch (std::exception &err) {
    std::cout << "exception:" << err.what() << std::endl;
  } catch (...) {
    std::cout << "exception but do nothing" << std::endl;  // do nothing
    throw;                                                 //重新抛出
  }
  return 0;
}
