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
static void Attributes(TC& myContainer) {
  if (!myContainer.empty()) {
    PRINT("container size %ld", myContainer.size());
    PRINT("myContainer.capacity() = %ld", myContainer.capacity());
    COM_BUF(myContainer.data(), myContainer.size());
  } else {
    PRINT("container is empty");
  }
}
//
// http://www.cplusplus.com/reference/forward_list/forward_list/
// 前向列表是序列容器，允许在序列的任何位置进行常量时间的插入和删除操作

static void BasicTest(void) {
  PRINT("*************** %s ****************", "forward_list constructor test");
  std::forward_list<int> first;                                // default: empty
  std::forward_list<int> second(3, 77);                        // fill: 3 seventy-sevens
  std::forward_list<int> third(second.begin(), second.end());  // range initialization
  std::forward_list<int> fourth(third);                        // copy constructor
  std::forward_list<int> fifth(std::move(fourth));             // move ctor. (fourth wasted)
  std::forward_list<int> sixth = {3, 52, 25, 90};              // initializer_list constructor

  std::cout << "first:";
  for (int& x : first) std::cout << " " << x;
  std::cout << '\n';
  std::cout << "second:";
  for (int& x : second) std::cout << " " << x;
  std::cout << '\n';
  std::cout << "third:";
  for (int& x : third) std::cout << " " << x;
  std::cout << '\n';
  std::cout << "fourth:";
  for (int& x : fourth) std::cout << " " << x;
  std::cout << '\n';
  std::cout << "fifth:";
  for (int& x : fifth) std::cout << " " << x;
  std::cout << '\n';
  std::cout << "sixth:";
  for (int& x : sixth) std::cout << " " << x;
  std::cout << '\n';

  // operator=
  std::forward_list<int> a(4);     // 4 ints
  std::forward_list<int> b(3, 5);  // 3 ints with value 5
  a = b;                           // copy assignment
  for (int& x : a) std::cout << ' ' << x;
  std::cout << '\n';
  for (int& x : b) std::cout << ' ' << x;
  std::cout << '\n';
}

struct student {
  int old;
  std::string name;
};

static void BasicElementTest(void) {
  PRINT("*************** %s ****************", "forward_list insert test");

  std::forward_list<struct student> list{};
  PRINT("list is empty %d, max size %ld", list.empty(), list.max_size());

  student tmp{1, "xiao wang"};
  list.insert_after(list.before_begin(), tmp);
  PRINT("list is empty %d, max size %ld", list.empty(), list.max_size());
  PRINT("after insert,tmp.old = %d, %s", tmp.old, tmp.name.data());

  student tmp1{2, "li san"};
  list.insert_after(list.before_begin(), tmp1);

  std::forward_list<struct student>::iterator y;

  // for( x=list.before_begin();x!=list.end();x++)
  // {
  //     PRINT("before_begin,old = %d, name = %s",x->old,x->name.data()); //core dump
  // }
  for (y = list.begin(); y != list.end(); y++) {
    PRINT("begin,old = %d, name = %s", y->old, y->name.data());
  }

  student tmp2{3, "zhao si"};
  list.push_front(tmp2);
  student tmp3{4, "wang wu"};
  list.push_front(tmp3);

  for (y = list.begin(); y != list.end(); y++) {
    PRINT("after push front,old = %d, name = %s", y->old, y->name.data());
  }

  // for( auto x=list.before_begin();x!=list.end();x++) //core dump
  // {
  //     if( (*(x++)).old == 2)
  //     {
  //         list.erase_after(x);
  //          PRINT("erase 2 after");
  //          break;
  //     }
  // }
  auto x = list.before_begin();
  for (y = list.begin(); y != list.end(); y++) {
    if ((*y).old == 2) {
      list.erase_after(x);
      break;
    }
    ++x;
  }

  for (y = list.begin(); y != list.end(); y++) {
    PRINT("erase 2 after,old = %d, name = %s", y->old, y->name.data());
  }

  // template < class T, class Alloc = allocator<T> > class forward_list;
  // value_type	The first template parameter (T)
  // void remove (const value_type& val);

  //只能删除值，不能操作迭代器,未能搞定
  // y=list.begin();
  // list.remove(std::make_pair(y->old,y->name));
  // for( y=list.begin();y!=list.end();y++)
  // {
  //     PRINT("remove 3 after,old = %d, name = %s",y->old,y->name.data());
  // }
}

void ForwardList(void) {
  BasicTest();
  BasicElementTest();
}

int main(int argc, char* argv[]) {
  try {
    VectorSizeTest();
    VectorAssignInsertTest();
  } catch (std::exception& err) {
    std::cout << "exception:" << err.what() << std::endl;
  } catch (...) {
    std::cout << "exception but do nothing" << std::endl;  // do nothing
    throw;                                                 //重新抛出
  }
  return 0;
}
