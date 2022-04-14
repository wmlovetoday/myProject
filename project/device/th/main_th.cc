

#include <getopt.h>
#include <pthread.h>
#include <stdint.h>

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <set>
#include <string>
#include <thread>
#include <vector>

#include "base_context.h"
#include "base_value.h"
#include "com_interface.h"
#include "common_log.h"
#include "log_printf.h"
#include "sys_th.h"

#define STR_FLAG "main"

using namespace sys;
int test = 0;
/** int32_t Regist(ThRunFunc &run_func, void *run_args);
 * // 如果是引用，不能把函数转换成functional类型
 *
 * no known conversion for argument 1 from ‘int32_t(bool&, bool&,
 * void*) {aka int(bool&, bool&, void*)}’ to ‘sys::ThRunFunc& {aka std::function<int(bool&, bool&, void*)>&}’
 */

int32_t AcquireThread(const bool &run, const bool &start, void *args) {
  uint8_t count = 0;
  while (run) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    if (start) {
      count++;
      PRINT("count %d", count);
    }
  }
  test = 1;
  PRINT("exit");
  return 0;
}

ThRunFunc rf = [](const bool &run, const bool &start, void *args) -> int32_t {
  AcquireThread(run, start, args);
  return 0;
};

static void ThreadProjectTest() {
  base_con::PrintFlag("Thread Project Test");
  Th th{};
  th.Regist(rf, nullptr);
  // th.Regist(AcquireThread, nullptr);
  th.StartCallback();
  uint32_t count = 0;
  while (count < 5) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    count++;
  }
  th.StopCallback();
  sleep(3);
  th.Remove();

  sleep(1);
  PRINT("TEST = %d", test);
  sleep(1);
}

int main(int argc, char *argv[]) {
  try {
    ThreadProjectTest();
  } catch (std::exception &err) {
    std::cout << "exception:" << err.what() << std::endl;
  } catch (...) {
    std::cout << "exception but do nothing" << std::endl;  // do nothing
    throw;                                                 //重新抛出
  }
  return 0;
}
