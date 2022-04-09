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

// http://www.cplusplus.com/reference/forward_list/forward_list/
// 前向列表是序列容器，允许在序列的任何位置进行常量时间的插入和删除操作

struct student {
  int old;
  std::string name;
};

static void BasicElementTest(void) {
  base_con::PrintFlag("Forward　List basic test");
  std::forward_list<struct student> list{};
  student tmp{1, "xiao wang"};
  list.insert_after(list.before_begin(), tmp);
  student tmp1{2, "li san"};
  list.insert_after(list.before_begin(), tmp1);

  std::forward_list<struct student>::iterator y;
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

/**
explicit list (const allocator_type& alloc = allocator_type());
explicit list (size_type n);
         list (size_type n, const value_type& val,
                const allocator_type& alloc = allocator_type());
template <class InputIterator>
  list (InputIterator first, InputIterator last,
         const allocator_type& alloc = allocator_type());
*/

static void ListConstructor() {
  base_con::PrintFlag("List Constructor test");
  // constructors used in the same order as described above:
  std::list<int> first;           // empty list of ints
  std::list<int> second(4, 100);  // four ints with value 100
  std::list<int> third(second.begin(),
                       second.end());  // iterating through second
  std::list<int> fourth(third);        // a copy of third

  // the iterator constructor can also be used to construct from arrays:
  int myints[] = {16, 2, 77, 29};
  std::list<int> fifth(myints, myints + sizeof(myints) / sizeof(int));
}

static void ForwardListTest(void) {
  base_con::PrintFlag("Forward List test");
  std::forward_list<int> first;          // default: empty
  std::forward_list<int> second(3, 77);  // fill: 3 seventy-sevens
  std::forward_list<int> third(second.begin(),
                               second.end());       // range initialization
  std::forward_list<int> fourth(third);             // copy constructor
  std::forward_list<int> fifth(std::move(fourth));  // move ctor. (fourth wasted)
  std::forward_list<int> sixth = {3, 52, 25, 90};   // initializer_list constructor

  // operator=
  std::forward_list<int> a(4);     // 4 ints
  std::forward_list<int> b(3, 5);  // 3 ints with value 5
  a = b;                           // copy assignment
}

// void pop_front(); list same with forward_list
static void ListPush() {
  base_con::PrintFlag("List Push test");
  std::list<int> mylist(2, 100);  // two ints with a value of 100
  mylist.push_front(200);         // emplace_front
  mylist.push_back(300);          // emplace_back

  base_con::PrintFlag("");
  mylist.pop_front();
  PRINT("after pop front");

  mylist.pop_back();
  base_con::PrintFlag("");
  PRINT("after pop back");
}

/**
  iterator insert (const_iterator position, const value_type& val);
  iterator insert (const_iterator position, size_type n, const value_type& val);
  template <class InputIterator>
  iterator insert (const_iterator position, InputIterator first, InputIterator
  last); iterator insert (const_iterator position, value_type&& val); iterator
  insert (const_iterator position, initializer_list<value_type> il);
*/
static void ListInsert() {
  base_con::PrintFlag("List Insert test");
  std::list<int> mylist;

  // set some initial values:
  for (int i = 1; i <= 5; ++i) {
    mylist.push_back(i);  // 1 2 3 4 5
  }

  /**
  list inster at begin, end very good; fix location bad.
  */
  mylist.insert(mylist.begin(), 10);
  // mylist.insert(mylist.begin(), 2, 8);
  std::list<int>::iterator it = mylist.begin();
  it++;
  mylist.insert(it, 2, 8);
}
/**
因为是单向向前链表，因此没有后面的相关操作．比如，无 push_back,但是有push_front
除了有list insert　一切功能外，还有更方便的，指定位置插入操作．
iterator insert_after ( const_iterator position, const value_type& val );
iterator insert_after ( const_iterator position, value_type&& val );
iterator insert_after ( const_iterator position, size_type n, const value_type&
val ); template <class InputIterator> iterator insert_after ( const_iterator
position, InputIterator first, InputIterator last ); iterator insert_after (
const_iterator position, initializer_list<value_type> il );
*/
static void ForwardListInsert() {
  base_con::PrintFlag("Forward List Insert test");
  std::forward_list<int> mylist;

  // set some initial values:
  for (int i = 1; i <= 5; ++i) {
    mylist.push_front(i);  // 1 2 3 4 5
  }
  mylist.insert_after(mylist.begin(), 0);
  base_con::PrintFlag("");
  PRINT("AFTER : mylist.insert_after(mylist.begin(), 0);");

  mylist.insert_after(mylist.before_begin(), 0);
  base_con::PrintFlag("");
  PRINT("AFTER : mylist.insert_after(mylist.before_begin(), 0);");
}

/**
void remove (const value_type& val); //list same with forward_list
*/
struct SensorAttrs {
 public:
  std::string serial;  // number id ? <query>
  std::string src_ipaddr;
  std::string dst_ipaddr;
  uint16_t src_port;
  uint16_t dst_port;
};

static void ForwardListEraseTest() {
  base_con::PrintFlag("Forward List erase test");
  std::forward_list<SensorAttrs> mylist;

  SensorAttrs tmp{"test_camera", "192.168.1.21", "192.168.1.21", 9000, 9001};
  SensorAttrs tmp1{"debug_camera", "192.168.1.21", "192.168.1.21", 8000, 8001};
  mylist.insert_after(mylist.before_begin(), tmp);
  mylist.insert_after(mylist.before_begin(), tmp1);

  auto x = mylist.before_begin();
  for (auto& t : mylist) {
    if ("debug_camera" == t.serial) {
      mylist.erase_after(x);
      // mylist.remove(x);  // remove only support value_type
    } else {
      x++;
    }
  }
  for (auto t : mylist) {
    std::cout << "--------------" << std::endl;
    std::cout << "serial : " << t.serial << std::endl;
    std::cout << "src_ipaddr : " << t.src_ipaddr << std::endl;
    std::cout << "dst_ipaddr : " << t.dst_ipaddr << std::endl;
    std::cout << "src_port : " << t.src_port << std::endl;
    std::cout << "dst_port : " << t.dst_port << std::endl;
  }
}

static void ForwardListRemoveTest() {
  base_con::PrintFlag("Forward List remove test");

  struct std::forward_list<int> mylist;

  // set some initial values:
  for (int i = 1; i <= 5; ++i) {
    mylist.push_front(i);  // 1 2 3 4 5
  }
  base_con::PrintFlag("");
  mylist.remove(3);
  base_con::PrintFlag("");
  PRINT("AFTER remove :");
}

/**
  //list
  iterator erase (const_iterator position);
  iterator erase (const_iterator first, const_iterator last);

  //ForwardList
  iterator erase_after (const_iterator position);
  iterator erase_after (const_iterator position, const_iterator last);
*/
static void ForwardListErase1Test() {
  base_con::PrintFlag("Forward List Erase test");
  std::forward_list<int> mylist;

  // set some initial values:
  for (int i = 1; i <= 5; ++i) {
    mylist.push_front(i);  // 1 2 3 4 5
  }
  base_con::PrintFlag("");
  mylist.erase_after(mylist.begin());
  base_con::PrintFlag("");
  PRINT("AFTER erase :");
}

int main(int argc, char* argv[]) {
  try {
    ListConstructor();
    ListPush();
    ListInsert();

    ForwardListTest();
    ForwardListTest();
    ForwardListRemoveTest();
    ForwardListEraseTest();
    ForwardListErase1Test();
    BasicElementTest();

  } catch (std::exception& err) {
    std::cout << "exception:" << err.what() << std::endl;
  } catch (...) {
    std::cout << "exception but do nothing" << std::endl;  // do nothing
    throw;                                                 //重新抛出
  }
  return 0;
}
