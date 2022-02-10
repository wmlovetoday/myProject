#include <cassert>   //assert
#include <exception> //static_assert
#include <iostream>
#include <iterator>
#include <string>
#include <thread>

#include "base_context.h"
#include "common_log.h"
#include "log_printf.h"

#include "th.h"

static void SleepFunc(int ms) {
  std::thread::id thread_thread_id = std::this_thread::get_id();
  PRINT("thread id %ld", thread_thread_id);
  // std::this_thread::sleep_for(std::chrono::milliseconds(ms));
  // PRINT("sleep %d ms", ms);
  std::this_thread::sleep_for(std::chrono::seconds(ms));
  PRINT("sleep %d s", ms);
}

static void ThreadAttributesTest() {
  base_con::PrintFlag("Thread AttributesTest Test");

  std::thread::id func_thread_id = std::this_thread::get_id();
  PRINT("func thread id %ld", func_thread_id);

  std::thread th{SleepFunc, 100};
  auto tn = th.native_handle();             // can use it for cancle
  PRINT("th.joinable()=%d", th.joinable()); // return bool type
  if (th.joinable()) {
    pthread_cancel(tn);
    th.join();
  }

  int sleep_ms = 200;
  auto fun = [](int ms) -> int {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    PRINT("sleep %d ms", ms);
    return 0;
  };
  std::thread th1; //只代表一个线程对象，不可执行
  th1 =
      std::thread{fun, sleep_ms}; // move-assign threads, std::thread::operator=
  PRINT("th1.joinable()=%d", th1.joinable()); // return bool type
  if (th1.joinable()) {
    th1.join();
  }
}

// 线程分离:允许线程自己独立运行不再受调用线程的约束(return 也不会结束线程了)
// after detach,　the thread object becomes non-joinable and can be destroyed
// safely.

static void ThreadDetachTest() {
  base_con::PrintFlag("Thread Detach Test");

  std::thread(SleepFunc, 1).detach();
  std::thread(SleepFunc, 2).detach();
  std::thread(SleepFunc, 3).detach();
  PRINT("now main exit,but thread is running...");
}

static void ThreadProjectTest() {
  base_con::PrintFlag("Thread Project Test");
  Th th{};
  th.Regist([](const uint8_t *data, uint32_t size) -> int32_t {
    PRINT("data[0]=%d, size= %d", data[0], size);
    return 0;
  });
  th.StartCallback();
  uint32_t count = 0;
  while (count < 5) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    count++;
  }
  th.StopCallback();
}

int main(int argc, char *argv[]) {

  try {
    ThreadAttributesTest();
    ThreadProjectTest();
  } catch (std::exception &err) {
    std::cout << "exception:" << err.what() << std::endl;
  } catch (...) {
    std::cout << "exception but do nothing" << std::endl; // do nothing
    throw;                                                //重新抛出
  }
  return 0;
}
