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
#include "template.hpp"

// #include "template.h"
// undefined reference to `wm::Swap(int&, int&)'

/**
:编译器会对函数模板进行两次编译:
            | 在声明的地方对模板代码本身进行编译;
            | 在调用的地方对参数替换后的代码进行编译;

因此在工程里面如果只包含.h文件在链接时会出错,因为第一次编译只进行语法的检验,而不生成函数体,在main中再次调用模板函数时,
会无法在此文件中链接,因此还要把.CPP包含进来,同时在.cpp里包含.h文件,显然把.cpp改写成.hpp,符合平时书写习惯；
*/

static void TemplateFunctionTest() {
  base_con::PrintFlag("Template Function test");

  int32_t a = 3;
  int32_t b = 10;
  wm::Swap(a, b);
  wm::Swap<int32_t>(a, b);
}

static void TemplateClassTest() {
  base_con::PrintFlag("Template Class test");
  wm::Test<std::vector<int>, int> te{};
  // wm::Test<std::vector<int>> te{}; // build failed
  std::vector<int> ret = te.GetIns(2);
  PRINT("te[0] = %d", ret[0]);
}
int main(int argc, char *argv[]) {
  try {
    TemplateFunctionTest();
    TemplateClassTest();
  } catch (std::exception &err) {
    std::cout << "exception:" << err.what() << std::endl;
  } catch (...) {
    std::cout << "exception but do nothing" << std::endl;  // do nothing
    throw;                                                 //重新抛出
  }
  return 0;
}
