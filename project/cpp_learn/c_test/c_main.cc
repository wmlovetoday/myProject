#include <cassert>  //assert
#include <condition_variable>
#include <exception>  //static_assert
#include <iostream>
#include <iterator>
#include <mutex>  // std::mutex, std::lock_guard
#include <string>
#include <vector>

#include "base_context.h"
#include "common_log.h"
#include "log_printf.h"

void SwitchTest() {
  uint32_t type = 1;
  uint32_t type_c = 1;
  switch (type) {
    case 1:
      type_c = type;
      break;
    case 2:
      type_c = type;
      break;
    default:
      type_c = 0;
      break;
  }
  PRINT("type = %d", type_c);
}

int main(int argc, char *argv[]) {
  try {
    SwitchTest();
  } catch (std::exception &err) {
    std::cout << "exception:" << err.what() << std::endl;
  } catch (...) {
    std::cout << "exception but do nothing" << std::endl;  // do nothing
    throw;                                                 //重新抛出
  }
  return 0;
}
