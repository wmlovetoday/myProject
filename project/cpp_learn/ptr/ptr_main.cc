#include <cassert>   //assert
#include <exception> //static_assert
#include <iostream>
#include <iterator>
#include <memory>
#include <string>

#include "base_context.h"
#include "common_log.h"
#include "log_printf.h"

/**
  template <class T, class D = default_delete<T>> class unique_ptr;
  template <class T, class D> class unique_ptr<T[],D>;
*/
/**
  unique_ptr not support copy, can only move.
*/

/**
template <class T> class default_delete;
template <class T> class default_delete<T[]>;

constexpr default_delete() noexcept = default;

template <class U, class = typename
enable_if<is_convertible<U*,T*>::value>::type> default_delete (const
default_delete<U>& del) noexcept {}
*/

template <typename TC> static void UniqueAttributes(TC &myContainer) {
  if (myContainer) {
    PRINT(" myContainer.get() = %ld, *myContainer = %d", myContainer.get(),
          *myContainer);
  } else {
    PRINT("memory is empty");
  }
}

template <typename TC> static void SharedAttributes(TC &myContainer) {
  if (myContainer) {
    PRINT(" myContainer.get() = %ld, *myContainer = %d use_count = %d",
          myContainer.get(), *myContainer, myContainer.use_count());
  } else {
    PRINT("memory is empty");
  }
}

static void ConstructorUniquePtrTest() {
  base_con::PrintFlag(" Constructor UniquePtr Test");
  std::default_delete<int> d{};
  std::unique_ptr<int> u1;
  std::unique_ptr<int> u2(nullptr);
  std::unique_ptr<int> u3(new int);
  std::unique_ptr<int> u4(new int, d);
  std::unique_ptr<int> u5(new int, std::default_delete<int>());
  std::unique_ptr<int> u6(std::move(u5));
  std::unique_ptr<int> u7(std::move(u6));
  std::unique_ptr<int> u8(std::auto_ptr<int>(new int));
}

/**
unique_ptr 包含了一个指针和一个释放器．直接对 unique_ptr
对象取址不具意义，必须使用get()方法． 可以对*get()方法进行赋值，用于改变变量值．
*/

static void OperatorEqualUniquePtrTest() {
  base_con::PrintFlag(" Operator Equal UniquePtr Test");
  std::unique_ptr<int> foo{};
  std::unique_ptr<int> bar{};
  PRINT("foo attr:");
  // foo = std::unique_ptr<int>(new int(101));
  foo = std::make_unique<int>(101);
  // foo = new int(101); /** 不可以直接赋值，因为 foo 并不只是简单的int 指针*/
  UniqueAttributes(foo);
  bar = std::move(foo);
  *bar.get() = 102;
  PRINT("bar attr:");
  UniqueAttributes(bar);
  PRINT("after bar=move(foo):");
  UniqueAttributes(foo);

  std::unique_ptr<int[]> ptr(new int[10]);
  // unique_ptr<int[]> 使用 default_delete<int[]>
}

template <typename T> static void Delete(T p) {
  delete p;
  PRINT("i am Delete func");
}

static void DeletePtrTest() {
  base_con::PrintFlag(" delete UniquePtr Test");
  auto del = [](int *p) {
    delete p;
    PRINT("i am delete func");
  };
  std::unique_ptr<int, decltype(del)> foo(new int, del);

  auto dell = [](int *p) { Delete(p); };
  std::unique_ptr<int, decltype(dell)> fooo(new int, dell);

  int *t = new int(2); //申请时即可初始化
  PRINT("*t = %d, t = 0x%lx", *t, reinterpret_cast<uint64_t>(t));
  delete t;
  PRINT("after delete, *t = %d, t = 0x%lx", *t, reinterpret_cast<uint64_t>(t));

  int *pp = new int[5]{0, 1, 2, 3, 4};
  /*for (int i = 0; i < 5; i++) {
    *(pp + i) = i;
  }*/
  PRINT("pp = 0x%lx", reinterpret_cast<uint64_t>(pp));
  delete[] pp;
  // pp = nullptr; // must assign to nullptr manual
  for (int j = 0; j < 5; j++) {
    PRINT("after delete, %d : %d", j, *(pp + j));
  }
  if (pp != nullptr) {
    PRINT("after delete, pp = 0x%lx", reinterpret_cast<uint64_t>(pp));
  }

  // 11:46:50 129734  ptr_main.cc:95 *t = 2, t = 0x153b9f0
  // 11:46:50 129738  ptr_main.cc:97 after delete, *t = 22264320, t = 0x153b9f0
  // 11:46:50 129741  ptr_main.cc:103 pp = 0x153b9f0
  // 11:46:50 129744  ptr_main.cc:106 after delete, 0 : 22264320
  // 11:46:50 129748  ptr_main.cc:106 after delete, 1 : 0
  // 11:46:50 129751  ptr_main.cc:106 after delete, 2 : 2
  // 11:46:50 129754  ptr_main.cc:106 after delete, 3 : 3
  // 11:46:50 129757  ptr_main.cc:106 after delete, 4 : 4
  // 11:46:50 129760  ptr_main.cc:109 after delete, pp = 0x153b9f0
  // 可见，delete后，指针内容不并不nullptr,也不能通过nullptr进行判断．也必须如c中的使用，手动设置为nullptr.
}

static void ResetReleaseUniquePtrTest() {
  base_con::PrintFlag(" Reset Release UniquePtr Test");
  std::unique_ptr<int> up; // empty

  std::unique_ptr<int> upp = std::make_unique<int>(11);
  up.reset(upp.get()); // Destroys the object currently managed by the
                       // unique_ptr (if any) and takes ownership of p.
  *up.get() = 12;
  PRINT("upp attr:");
  UniqueAttributes(upp);
  // up.reset(); //
  PRINT("up attr:");
  UniqueAttributes(up); // up.get() == upp.get(), double free and corruption
  up.release(); // Releases ownership of its stored pointer, by returning its
                // value and replacing it with a null pointer.
                // This call does not destroy the managed object
  //上面的执行过程，up.get() == upp.get(), 执行 up.release()
  //结束up对upp的指针管理，并且不调用释放方法．
  //释放由upp自己完成．如果不执行 up.release()，double free and corruption

  // reset()，结束指针管理，并且调用释放函数，所以会 double free
  // release()，结束指针管理，不调用释放函数，所以不会 double free
}

struct Student {
  int old;
  char sex;
};
static void ReferenceUniquePtrTest() {
  base_con::PrintFlag(" Reference UniquePtr Test");

  // Student s{20, 'B'};
  // std::unique_ptr<Student> stu = std::make_unique<Student>(s);
  std::unique_ptr<Student> stu = std::make_unique<Student>(Student{20, 'B'});
  PRINT("old : %d, sex : %c", stu->old, stu->sex);

  std::unique_ptr<int> upp = std::make_unique<int>(11);
  *upp = 12;
}

static void MovePtrTest() {
  base_con::PrintFlag(" move UniquePtr Test");
  std::unique_ptr<int> foo = std::unique_ptr<int>(new int(101));

  std::unique_ptr<int> bar = std::move(foo);
  PRINT("after move foo attr:"); // is empty
  UniqueAttributes(foo);
  PRINT("bar attr:");
  UniqueAttributes(bar);
}

class UniTest {
public:
  explicit UniTest(std::string serial) : ser_(serial) {
    PRINT("constructor ser %s", ser_.data());
  }
  int32_t DataParse(char *data, int size) {
    PRINT(" ser %s size %d", data, size);
    return 0;
  }
  ~UniTest() {
    PRINT("destructor ser %s", ser_.data());
    ser_.clear();
  }

private:
  std::string ser_{};
};

static void UniqueAssignTest(void) {
  base_con::PrintFlag(" Unique Assign Ptr Test");

  std::unique_ptr<UniTest> ut{new UniTest("HELLO")};
  std::unique_ptr<UniTest> uut{};
  uut = std::make_unique<UniTest>(UniTest("world"));

  // 13:52:23 957090  ptr_main.cc:188 constructor ser HELLO
  // 13:52:23 957093  ptr_main.cc:188 constructor ser  world
  // 13:52:23 957095  ptr_main.cc:194 destructor ser  world
  // 13:52:23 957098  ptr_main.cc:194 destructor ser  world  //destruct
  // two,因为中间一有次默认移动构造 times 13:52:23 957100  ptr_main.cc:194
  // destructor ser HELLO

  std::unique_ptr<UniTest> t{new UniTest("hi")};
  std::unique_ptr<UniTest> tt = std::move(t);
  //只有一次构造与虚构,所以可以多多使用move，并且Move只是把
  //一个左传强制转换成右值． 因此不可以把局部变量Move给全局变量
  // 14:0:46 541328  ptr_main.cc:188 constructor ser hi
  // 14:0:46 541332  ptr_main.cc:195 destructor ser hi

  // std::unique_ptr<UniTest> p = std::move(new UniTest("HELLO"));
  // Notice that this function only performs move assignment. Copy assignment is
  // not supported by unique_ptr failed:  no match for ‘operator=’ (operand
  // types are ‘std::unique_ptr<UniTest>’ and
  // ‘std::remove_reference<UniTest*>::type {aka UniTest*}’)
}
static void SharedPtrCountTest() {
  base_con::PrintFlag(" shared Ptr Test");
  // std::shared_ptr<int> sh(new int(5));
  std::shared_ptr<int> sh = std::make_shared<int>(5);
  SharedAttributes(sh);
  std::shared_ptr<int> ssh = sh;
  SharedAttributes(ssh);
}

static void MakeShared(void) {
  base_con::PrintFlag("shared ptr (make_shared) test");
  std::shared_ptr<int> foo = std::make_shared<int>(10);
  // same as:
  std::shared_ptr<int> foo2(new int(10));
  // shared_ptr<int> sp1 = new int(100); // error 不会进行隐式转换，类型不符合

  auto bar = std::make_shared<int>(20);
  auto baz = std::make_shared<std::pair<int, int>>(30, 40);

  std::cout << "*foo: " << *foo << '\n';
  std::cout << "*bar: " << *bar << '\n';
  std::cout << "*baz: " << baz->first << ' ' << baz->second << '\n';
}

// 一个典型的错误用法,将一个祼指针为 shared_ptr 赋值
// 只有用一个shared_ptr为另一个shared_ptr赋值时，才将这连个共享指针关联起来

// 不要保存p.get()的返回值
// 无论是保存为裸指针还是shared_ptr都是错误的
// 保存为裸指针不知什么时候就会变成空悬指针
// 保存为shared_ptr则产生了独立指针
// 不要delete p.get()的返回值
// 会导致对一块内存delete两次的错误

/********************* shared ptr ***********************/
/** constructor and destructor same with unique ptr*/

int main(int argc, char *argv[]) {

  try {
    ConstructorUniquePtrTest();
    OperatorEqualUniquePtrTest();
    ResetReleaseUniquePtrTest();
    DeletePtrTest();
    ReferenceUniquePtrTest();
    MovePtrTest();
    UniqueAssignTest();

    SharedPtrCountTest();
    MakeShared();
  } catch (std::exception &err) {
    std::cout << "exception:" << err.what() << std::endl;
  } catch (...) {
    std::cout << "exception but do nothing" << std::endl; // do nothing
    throw;                                                //重新抛出
  }
  return 0;
}
