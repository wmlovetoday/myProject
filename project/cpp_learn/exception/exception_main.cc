#include <cassert>   //assert
#include <exception> //static_assert
#include <iostream>
#include <string>

#include "base_context.h"
#include "common_log.h"
#include "log_printf.h"

static int DoTask() { throw std::runtime_error{"log category failed"}; }

int main(int argc, char *argv[]) {
  base_con::PrintFlag("exception test");

  PTS_INFO("i am zlog !!!");
  PRINT("i am C log !!!");

  try {
    DoTask();
  } catch (std::exception &err) {
    std::cout << "exception:" << err.what() << std::endl;
  } catch (...) {
    std::cout << "exception but do nothing" << std::endl; // do nothing
    throw;                                                //重新抛出
  }
  return 0;
}
