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

// vector 的大多数性质都与 string 相同，可以去看相同目录下的string.

template <typename TC>
static void Attributes(TC &myContainer) {
  if (!myContainer.empty()) {
    PRINT("container size %ld", myContainer.size());
    PRINT("myContainer.capacity() = %ld", myContainer.capacity());
    COM_BUF(myContainer.data(), myContainer.size());
  } else {
    PRINT("container is empty");
  }
}

/**
Change size
  void resize (size_type n);
  void resize (size_type n, const value_type& val);

Request a change in capacity. 使用条件较苛刻，不用为好
有可能申请失败，一旦成功，则　capacity　可能会大于申请数量．
  void reserve (size_type n);

返回值均为void,因此是否申请成功，要自己判断．
*/

void VectorSizeTest() {
  base_con::PrintFlag("Vector Size test");

  std::vector<char> s1{};
  s1.resize(16, 1);
  PRINT("s1.resize(16,1) : ");
  Attributes(s1);

  // vector_main.cc:40 s1.resize(16,1) :
  // vector_main.cc:19 container size 16
  // vector_main.cc:20 myContainer.capacity() = 16

  s1.clear();
  s1.reserve(32);
  s1.push_back('a');
  base_con::PrintFlag("");
  PRINT("s1.reserve(16) : ");
  Attributes(s1);

  // vector_main.cc:47 s1.reserve(16) :
  // vector_main.cc:19 container size 1
  // vector_main.cc:20 myContainer.capacity() = 32
}

/**
  no append, but assign．　// assign. 会顶替掉上一次的操作．
  template <class InputIterator>
    void assign (InputIterator first, InputIterator last);
  void assign (size_type n, const value_type& val);
  void assign (initializer_list<value_type> il);
*/
/**
  iterator insert (const_iterator position, const value_type& val);
  iterator insert (const_iterator position, size_type n, const value_type& val);
  template <class InputIterator>
  iterator insert (const_iterator position, InputIterator first, InputIterator last);
  iterator insert (const_iterator position, value_type&& val);
  iterator insert (const_iterator position, initializer_list<value_type> il);
 */
static void VectorAssignInsertTest(void) {
  base_con::PrintFlag("Vector Assign Insert Test");
  std::vector<char> s1{};
  std::vector<char> s2{};
  s1.assign(2, 1);
  s1.assign({4, 5, 6});  // 会顶替掉上一次的操作．

  base_con::PrintFlag("");
  PRINT("assign : ");
  Attributes(s1);

  s2.insert(s2.begin(), 3, 1);
  s2.insert(s2.begin(), s1.begin(), s1.end());
  PRINT("inster : ");
  Attributes(s2);
}
// todo
// 二维 vectoy
// string vectoy

int main(int argc, char *argv[]) {
  try {
    VectorSizeTest();
    VectorAssignInsertTest();
  } catch (std::exception &err) {
    std::cout << "exception:" << err.what() << std::endl;
  } catch (...) {
    std::cout << "exception but do nothing" << std::endl;  // do nothing
    throw;                                                 //重新抛出
  }
  return 0;
}
