#include <cassert> //assert
#include <condition_variable>
#include <exception> //static_assert
#include <iostream>
#include <iterator>
#include <mutex> // std::mutex, std::lock_guard
#include <string>

#include "base_context.h"
#include "common_log.h"
#include "log_printf.h"

// :RAII: Resource Acquisition Is Initialization 资源获取即初始化,自动构造与释放

//主要用于防止异常退出时，没有释放mutex
//在声明时　std::lock_guard<std::mutex> lck (mtx);　就已经给mtx上锁了,
//因为RAII,不可以手动lck.~lock_guard(),会coredump
//只能等生命周期消失才会自动解锁.
//因引使用它时必须使用尽可能小的作用域

std::mutex mtx{};

static void BasicMutexTest(void) {
  base_con::PrintFlag("basic mutex test");
  for (int tmp = 0; tmp < 10; tmp++) {
    mtx.lock();
    PRINT("tmp = %d", tmp);
    mtx.unlock();
    mtx.unlock();
  }
}

static int LockGuardTest() {
  base_con::PrintFlag("lock guard test");
  std::lock_guard<std::mutex> lck(mtx);
}

// unique_lock 可以解决上述的问题
//不需要在初始化时必须上锁
//会在生命周期结束时自动释放，当然也可以自己手动释放
// unique_lock对象是可以进行交换的

static void UniqueLockTest() {
  base_con::PrintFlag("unique lock test");
  // 如果不加任何标识, 构造即上锁．Lock on construction by calling member
  // lock.
  std::unique_lock<std::mutex> lck(mtx); //之后不可以再lock,声明即Lock
  lck.unlock();

  // 增加 defer_lock 标识 :	Do not lock on construction
  std::unique_lock<std::mutex> llck(mtx, std::defer_lock);
  llck.lock();
  llck.unlock();
}

// https://en.cppreference.com/w/cpp/thread/condition_variable

// The class std::condition_variable is a StandardLayoutType.
// It is not CopyConstructible, MoveConstructible, CopyAssignable, or
// MoveAssignable. 因此放到类中,会使类不可复制等等

void BasicVariableTest(void) {
  base_con::PrintFlag("condition variable test");
  int test = 5;
  std::mutex upd{};
  std::condition_variable update_var;

  std::unique_lock<std::mutex> ulck(upd);
  while (test) {
    if (update_var.wait_for(ulck, std::chrono::seconds(1)) ==
        std::cv_status::timeout) {
      PRINT(" %d ...", test);
      test--;
    }
  }
}

int main(int argc, char *argv[]) {

  try {
    BasicMutexTest();
    LockGuardTest();
    UniqueLockTest();
    BasicVariableTest();
  } catch (std::exception &err) {
    std::cout << "exception:" << err.what() << std::endl;
  } catch (...) {
    std::cout << "exception but do nothing" << std::endl; // do nothing
    throw;                                                //重新抛出
  }
  return 0;
}
