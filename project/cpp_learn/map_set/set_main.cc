#include <cassert> //assert
#include <cstdint>
#include <exception> //static_assert
#include <initializer_list>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "base_context.h"
#include "common_log.h"
#include "log_printf.h"
/**有序无序体现为： 遍历元素时是否按序．*/

// map<T1,T2> 	普通映射 	有序,键值唯一
// set<T> 	普通集合 	有序,没有重复的元素
// multimap 	多重映射 	有序,允许重复键值
// multiset 	多重集合 	有序,允许重复的元素
// <unordered_map> 	哈希关联映射 	无序,
// <unordered_set> 	哈希关联集合 	无序,

// Unorderset and set is no diff
template <typename TC> static void Attributes(TC &myContainer) {
  if (!myContainer.empty()) {
    PRINT("container size %ld", myContainer.size());
    PRINT("container max_size %ld", myContainer.max_size());
  } else {
    PRINT("container is empty");
  }
}

template <typename TC> static void OutSetValue(TC &myContainer) {
  // auto it = myContainer.begin();
  // std::unordered_set<int>::iterator it;
  std::uint32_t index = 0;
  for (auto it = myContainer.begin(); it != myContainer.end(); it++) {
    PTS_INFO("{} : {}", index, *it);
    index++;
  }
}

template <typename TC> static void OutMapValue(TC &myContainer) {
  // auto it = myContainer.begin();
  // std::unordered_set<int>::iterator it;
  std::uint32_t index = 0;
  for (auto it = myContainer.begin(); it != myContainer.end(); it++) {
    PTS_INFO("{} : {} => {}", index, it->first, it->second);
    index++;
  }
}
/*
模板类型是否可以嵌套?
不行，也没必要．因为一旦实例化，类型就是已知的．
所以可以把整个嵌套后的类型，整体作为一个模板．
 template <typename TC, typename TV>
static void Attributes(TC<TV> &myContainer) {
  if (!myContainer.empty()) {
    PRINT("container size %ld", myContainer.size());
    PRINT("container max_size %ld", myContainer.max_size());
  } else {
    PRINT("container is empty");
  }
}
 */

static void InitMapTest(void) {
  base_con::PrintFlag("map test");

  std::map<std::string, std::string> dic{{"exp", "hello world"},
                                         {"nihao", "hello"}};
  dic["sea"] = "large body of water"; // if map no "sea" key, will auto insert
                                      // it. otherwise not.
  // dic.at("sea") = "very large body of water"; // if no "sea" key, throws an
  // out_of_range exception.
  PRINT("dic['sea'] = %s", dic["sea"].data());
  Attributes(dic);
  OutMapValue(dic);
}
/**
(1)	single element
  pair<iterator,bool> insert (const value_type& val);
  template <class P> pair<iterator,bool> insert (P&& val);
(2) with hint
  iterator insert (const_iterator position, const value_type& val);
  template <class P> iterator insert (const_iterator position, P&& val);
(3) range
  template <class InputIterator>
  void insert (InputIterator first, InputIterator last);
(4) initializer list
  void insert (initializer_list<value_type> il);
*/
static void InsertMapTest(void) {
  base_con::PrintFlag("map insert test");
  std::map<std::string, std::string> dic{};

  dic.insert(
      std::pair<std::string, std::string>("season", "from spring to winter"));
  dic.insert(std::make_pair<std::string, std::string>("fruit",
                                                      "such as apple orange"));

  std::pair<std::map<std::string, std::string>::iterator, bool> tmp;
  tmp =
      dic.insert(std::pair<std::string, std::string>("week", "such as monday"));
  if (tmp.second == false) {
    PRINT("element 'week' already existed : %s", tmp.first->second.data());
  }
  auto q = dic.find("ocean");
  if (q == dic.end()) {
    PRINT("'ocean' not found");
    dic.emplace("ocean", "same to sea");
  } else {
    PRINT("'ocean' : %s", dic["ocean"].data());
  }
  OutMapValue(dic);

  auto fi = dic.find("week");
  if (fi == dic.end()) {
    PRINT("'week' not found");
  } else {
    PRINT("'week' : %s", dic["week"].data());
    dic.erase(fi);
  }
  auto fin = dic.find("week");
  if (fin == dic.end()) {
    PRINT("after erase 'week', 'week' not found");
  }
  OutMapValue(dic);
}

// 容器类可以检测出该类型是否“move aware”然后正确的进行处理
class Student {
public:
  std::string name;
  int old;
};

static void InsertUnordermapTest(void) {
  base_con::PrintFlag("Unorder map insert test");

  std::unordered_map<std::string, std::unique_ptr<Student>> name_book{};

  std::unique_ptr<Student> a1 = std::make_unique<Student>(Student());
  a1->name = "wang";
  a1->old = 20;
  name_book.insert(std::make_pair(
      std::string("1"), std::move(a1))); // make_pair is move insertion
  Attributes(name_book);
  // name_book.insert(std::make_pair(std::string("1"),a1.get() ));
}

class A {
public:
  // virtual void prn() = 0;
  // virtual void prn() = 0;
  A() { std::cout << "A\n"; }
  virtual void prn() { std::cout << "prn : A\n"; }
  virtual ~A() { std::cout << "~A\n"; }
};

class B : public A {
public:
  B() { std::cout << "B\n"; }
  void prn() { std::cout << "prn : B\n"; }
  ~B() { std::cout << "~B\n"; }
};

void CreatUnorderMap(void) {
  base_con::PrintFlag("Unorder map insert test");
  std::unordered_map<char *, std::unique_ptr<A>> map{};
  std::unique_ptr<B> b1 = std::make_unique<B>(B());
  PRINT("insert to map");
  map.insert(std::make_pair<char *, std::unique_ptr<A>>(
      const_cast<char *>("wang"), std::move(b1)));

  using dev_t = std::unordered_map<char *, std::unique_ptr<A>>::iterator;

  dev_t tmp = map.find(const_cast<char *>("wang"));
  if (tmp != map.end()) {
    tmp->second->prn();
  }
}

/** insert
single element (1) pair<iterator,bool> insert (const value_type& val);
                   pair<iterator,bool> insert (value_type&& val);
with hint (2)
  iterator insert (const_iterator position, const value_type& val);
  iterator insert (const_iterator position, value_type&& val);

range (3)
  template <class InputIterator> void insert (InputIterator first, InputIterator
last);

initializer list (4) void insert (initializer_list<value_type> il);
*/
// erase
// (1)	iterator  erase (const_iterator position);
// (2)	size_type erase (const value_type& val);
// (3)	iterator  erase (const_iterator first, const_iterator last);

// iterator find (const value_type& val);
static void InitSetTest(void) {
  base_con::PrintFlag("set test");

  std::set<int> myset{};
  std::pair<std::set<int>::iterator, bool> ret;

  myset.insert(std::initializer_list<int>{4, 10, 6, 12}); // (6) return void
  PRINT(" init set : ");
  Attributes(myset);
  OutSetValue(myset);

  ret = myset.insert(3); // (2) default insert at begin
  // auto iter = myset.insert(myset.end(), 100);
  if (ret.second) {
    PRINT("insert value success,default insert at begin");
    Attributes(myset);
    /**
      0 : 3
      1 : 4
      2 : 6
      3 : 10
      4 : 12
      默认排序了．．．．．．
    */
    OutSetValue(myset);
  } else {
    PRINT("insert value success failed");
  }

  int f_value = 10;
  auto fin = myset.find(f_value);
  if (fin != myset.end()) {
    PRINT("find %d", *fin);
  } else {
    PRINT("not find %d", f_value);
  }
}

/*
(1) pair<iterator, bool> insert(const value_type &val);
(2) pair<iterator, bool> insert(value_type &&val);
(3) iterator insert(const_iterator hint, const value_type &val);
(4) iterator insert(const_iterator hint, value_type &&val);
(5) template <class InputIterator>
  void insert(InputIterator first, InputIterator last);
(6) void insert(initializer_list<value_type> il);
*/

static void InsertUnorderSetTest() {
  base_con::PrintFlag("unorder set test");

  std::pair<std::unordered_set<int>::iterator, bool> ret;
  std::initializer_list<int> tmp{4, 10, 6, 12, 5, 7, 9, 25};
  std::unordered_set<int> myset{};
  // myset.insert(std::initializer_list<int>{4, 10, 6, 12, 5, 7, 9, 25});
  myset.insert(tmp.begin(), tmp.end()); // (6) return void
  PRINT(" init unordered set : ");
  Attributes(myset);
  OutSetValue(myset);
  /**
    0 : 12
    1 : 6
    2 : 10
    3 : 4  说明，大概率也是一个一个插入的，而且每次从set.begin()开始插入的
  */

  ret = myset.insert(100); // (2) default insert at begin
  myset.insert(3);
  // auto iter = myset.insert(myset.end(), 100);
  if (ret.second) {
    PRINT("insert value success,default insert at begin");
    Attributes(myset);
    /**
      0 : 4
      1 : 10
      2 : 6
      3 : 100
      4 : 12 这可能就是 unordered　的真谛吧,
      其实在指定位置insert，也不影响其排序
    */
    OutSetValue(myset);
  } else {
    PRINT("insert value success failed");
  }
}
/**
  by position (1)	iterator erase ( const_iterator position );
       by key (2)	size_type erase ( const key_type& k );
        range (3)	iterator erase ( const_iterator first, const_iterator
  last );
*/

// find
static void EraseUnorderSetTest() {
  base_con::PrintFlag("Erase unorder set test");
  std::unordered_set<int> myset{};
  std::initializer_list<int> tmp{4, 10, 6, 12, 14};
  myset.insert(tmp.begin(), tmp.end());
  auto iter = myset.erase(14);
  Attributes(myset);
  OutSetValue(myset);
  PTS_INFO("erase return : {}", iter); // 1, erase count

  myset.clear();
  PTS_INFO("after clear : {}", myset.empty()); // 1, erase count
}
int main(int argc, char *argv[]) {
  try {
    InitMapTest();
    InsertMapTest();
    InsertUnordermapTest();
    CreatUnorderMap();
    InitSetTest();
    InsertUnorderSetTest();
    EraseUnorderSetTest();
  } catch (std::exception &err) {
    PRINT("exception : %s", err.what());
  } catch (...) {
    PRINT("exception but do nothing"); // do nothing
    throw;                             //重新抛出
  }
  return 0;
}
