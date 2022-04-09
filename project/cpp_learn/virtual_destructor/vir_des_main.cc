#include <cassert>    //assert
#include <exception>  //static_assert
#include <iostream>
#include <iterator>
#include <memory>
#include <string>

#include "base_context.h"
#include "common_log.h"
#include "log_printf.h"

#define VIR_DES 0

class Father {
 public:
  Father() { PRINT("Father's constructor !"); }
  void print() { PRINT("Father's print !"); }
  virtual void DoSomething() { PRINT("Do something (virtual) in Father !"); };
#if VIR_DES == 1
  virtual ~Father() { PRINT("Father's virtual destructor !"); }
#else
  ~Father() { PRINT("Father's destructor !"); }
#endif
};

class Son : public Father {
 public:
  Son() { PRINT("Son's constructor !"); }
  void print() { PRINT("Son's print !"); }
  void DoSomething() override { PRINT("Do something (virtual) in Son !"); };
#if VIR_DES == 1
  virtual ~Son() { PRINT("Son's virtual destructor !"); }
#else
  ~Son() { PRINT("Son's destructor !"); }
#endif
};

// https://blog.csdn.net/weicao1990/article/details/81911341
// （1）如果父类的析构函数不加virtual关键字
// 当父类的析构函数不声明成虚析构函数的时候，当子类继承父类，父类的指针指向子类时，delete掉父类的指针，只调动父类的析构函数，而不调动子类的析构函数。
// （2）如果父类的析构函数加virtual关键字
// 当父类的析构函数声明成虚析构函数的时候，当子类继承父类，父类的指针指向子类时，delete掉父类的指针，先调动子类的析构函数，再调动父类的析构函数。

// 父子类之间的相互赋值，按定义类型去调用非虚函数，按实际类型去调用虚函数．
static void VirtualDestructorTest() {
  base_con::PrintFlag("virtual destructor test");

  PRINT("assign son to father, only exec father destructor, if father destructor is not virtual");
  Father *pTest = new Son;
  pTest->DoSomething();
  delete pTest;

  base_con::PrintFlag("");
  PRINT("assign son to son, exec son and father destructor, if father destructor is virtual");
  auto *Test = new Son;
  Test->DoSomething();
  delete Test;

  Father f1;
  Son s1;

  base_con::PrintFlag("");
  PRINT("assign son to son pointer, assign father to father pointer");
  Father *f = &f1;
  Son *s = &s1;

  f->print();
  f->DoSomething();
  s->print();
  s->DoSomething();

  base_con::PrintFlag("");
  PRINT("assign son to father pointer");
  Father *ff = &s1;
  ff->print();
  ff->DoSomething();

  base_con::PrintFlag("");
  PRINT("pointer cast between class");
  Father *fff = dynamic_cast<Father *>(&s1);  // static_cast / reinterpret_cast is also ok
  fff->print();
  fff->DoSomething();
  Son *ss = reinterpret_cast<Son *>(fff);
  ss->print();
  ss->DoSomething();
}

int main(int argc, char *argv[]) {
  try {
    VirtualDestructorTest();
  } catch (std::exception &err) {
    std::cout << "exception:" << err.what() << std::endl;
  } catch (...) {
    std::cout << "exception but do nothing" << std::endl;  // do nothing
    throw;                                                 //重新抛出
  }
  return 0;
}
