#include <cassert>  //assert
#include <condition_variable>
#include <exception>  //static_assert
#include <iostream>
#include <iterator>
#include <mutex>  // std::mutex, std::lock_guard
#include <string>

#include "base_context.h"
#include "common_log.h"
#include "log_printf.h"

// :RAII: Resource Acquisition Is Initialization 资源获取即初始化,自动构造与释放

// https://blog.csdn.net/m0_46657980/article/details/109385050
// 常规手段没办法访问类的私有成员，折衷，于是就有了 friend 关键字。
/**
 * 都知道传参时，如果直接传对象，那就是复制整个对象。传引用则只传对象的地址。
 * 对于函数返回值：
 * 　　不能直接返回对象引用，因为清栈后引用失效了。
 *　　　但如果返回对象，就要有拷贝，降低了性能。
 *     可以选择返回 shared_ptr  <1>
 * 　　
 * 但是还有另一种说法：
 *     可以放心大胆的返回对象，编译器会帮我们优化，会在值传递返回值的函数中，将返回值改写成传引用参数。这样可以少一次临时变量的构造。这是编译器优化时帮我们做的。
 *     如果不开优化，那会发生一次拷贝构造。
 *       编译器会采用RVO（return value
 *optimization）优化，参看《深度探索C++对象模型》
 */

/**
 * shared_ptr<T> fun()
 * {
 *     make_shared<T> ptr;
 *     return ptr;
 * }
 */

class Student {
 public:
  std::string name;
  int old;
  std::vector<int> score;
  ~Student() { PRINT("~Student()"); }
};

Student Factory(const std::string &na, int ol) {
  Student stu;
  stu.name = na;
  stu.old = ol;
  stu.score.push_back(10);
  stu.score.push_back(20);
  PRINT("&stu : %ld", &stu);
  return stu;
}
using PtrStudent = std::shared_ptr<Student>;
// std::make_shared<Student>;
PtrStudent PtrFactory(const std::string &na, int ol) {
  Student stu;
  stu.name = na;
  stu.old = ol;
  stu.score.push_back(10);
  stu.score.push_back(20);
  PtrStudent ptr_stu = std::make_shared<Student>(stu);
  return ptr_stu;
}

void ReturnClassTest() {
  base_con::PrintFlag("return class test");
  Student stu = Factory("ming", 20);
  PRINT("stu : %ld", &stu);
  PRINT("name : %s", stu.name.data());
  PRINT("old : %d", stu.old);
  PRINT("score : size %ld %d", stu.score.size(), stu.score[0]);
}

void ReturnClassPtrTest() {
  base_con::PrintFlag("return class ptr test");
  PtrStudent stu = PtrFactory("ming", 20);
  PRINT("stu : %ld", &stu);
  PRINT("name : %s", stu->name.data());
  PRINT("old : %d", stu->old);
  PRINT("score : size %ld %d", stu->score.size(), stu->score[0]);
}

int main(int argc, char *argv[]) {
  try {
    ReturnClassTest();
    ReturnClassPtrTest();
  } catch (std::exception &err) {
    std::cout << "exception:" << err.what() << std::endl;
  } catch (...) {
    std::cout << "exception but do nothing" << std::endl;  // do nothing
    throw;                                                 //重新抛出
  }
  return 0;
}
