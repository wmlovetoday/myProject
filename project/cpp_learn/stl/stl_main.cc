#include <cassert>  //assert
#include <condition_variable>
#include <exception>  //static_assert
#include <forward_list>
#include <iostream>
#include <iterator>
#include <list>
#include <mutex>  // std::mutex, std::lock_guard
#include <string>
#include <vector>

#include "base_context.h"
#include "common_log.h"
#include "log_printf.h"

struct Student {
  int old;
  std::string name;
};

using BaseList = std::forward_list<Student>;

class Base {
 public:
  BaseList GetStudent(int32_t type);
  void PrintList();
  void PrintList(const BaseList& list);

 private:
  BaseList sen_list_{};
};

BaseList Base::GetStudent(int32_t type) {
  Student stu{10, "XiaoMing"};
  Student stu1{11, "MeiMei"};
  sen_list_.insert_after(sen_list_.before_begin(), stu);
  sen_list_.insert_after(sen_list_.before_begin(), stu1);
  if (type == 1) {
    PRINT("return private sen_list_");
    return sen_list_;
  }

  if (type == 2) {
    PRINT("return li{sen_list_};");
    BaseList li{sen_list_};
    return li;
  }

  if (type == 3) {
    PRINT("return li{sen_list_};");
    BaseList li{};
    li.insert_after(li.before_begin(), sen_list_.begin(), sen_list_.end());
    return li;
  }
  if (type == 4) {
    PRINT("return li{sen_list_};");
    BaseList li{};
    li = sen_list_;
    return li;
  }
  std::string err{"///////////////// ERROR, never to be here"};
  throw std::runtime_error{err};
}

void Base::PrintList(const BaseList& list) {
  for (auto li : list) {
    std::cout << "---------" << std::endl;
    PRINT("old: %d", li.old);
    PRINT("name: %s", li.name.data());
  }
}
void Base::PrintList() {
  for (auto li : sen_list_) {
    std::cout << "+++++++++++" << std::endl;
    PRINT("old: %d", li.old);
    PRINT("name: %s", li.name.data());
  }
}

class A {
 public:
  A() { std::cout << "[C] constructor fired." << std::endl; }
  A(const A& a) { std::cout << "[C] copying constructor fired." << std::endl; }
  A(A&& a) { std::cout << "[C] moving copying constructor fired." << std::endl; }
  ~A() { std::cout << "[C] destructor fired." << std::endl; }
};

A getTempA() { return A(); }

/**
 * -fno-elide-constructors 如果关闭rvo,则发生两次构造和虚构，否则只有一次，默认打开rvo.
 */

int main(int argc, char** argv) {
  auto x = getTempA();
  return 0;
}

// int main(int argc, char* argv[]) {
//   try {
//     base_con::PrintFlag("forward_list test");
//     Base base{};

//     BaseList stu_list = base.GetStudent(std::stoi(argv[1]));

//     std::cout << " *************after erase " << std::endl;
//     stu_list.erase_after(stu_list.before_begin());
//     base.PrintList(stu_list);
//     base.PrintList();

//   } catch (std::exception& err) {
//     std::cout << "exception:" << err.what() << std::endl;
//   } catch (...) {
//     std::cout << "exception but do nothing" << std::endl;  // do nothing
//     throw;                                                 //重新抛出
//   }
//   return 0;
// }
