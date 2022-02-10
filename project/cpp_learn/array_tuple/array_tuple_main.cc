#include <array>
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

template <typename TC>
static void Attributes(TC &myContainer) {
  if (!myContainer.empty()) {
    PRINT("container size %ld", myContainer.size());
    COM_BUF(myContainer.data(), myContainer.size());
  } else {
    PRINT("container is empty");
  }
}

/**
  template < class T, size_t N > class array;
  void fill (const value_type& val);

  [] not check bounds of elements
  at() check bounds of elements
*/

void ArraySizeTest() {
  base_con::PrintFlag("Array Tuple Size test");

  std::array<char, 6> s1;
  s1.fill(5);
  PRINT("s1.fill(5); : ");
  Attributes(s1);
}

void ArrayAssertTest() {
  base_con::PrintFlag("Array Tuple assert test");

  std::array<char, 6> s1;
  s1.fill(6);
  s1[0] = 2;
  s1.at(1) = 3;
  Attributes(s1);

  // std::array<char, 6>::iterator tem = s1.begin();
  // for (unsigned int i = 0; i < s1.size(); i++) std::cout << ' ' << *(tem++);
}

void TupleConstructTest() {
  base_con::PrintFlag("Tuple Construct test");

  // constructor
  std::tuple<int, char> first;                            // default
  std::tuple<int, char> second(first);                    // copy
  std::tuple<int, char> third(std::make_tuple(20, 'b'));  // move
  std::tuple<long, char> fourth(third);                   // implicit conversion
  std::tuple<int, char> fifth(10, 'a');                   // initialization
  std::tuple<int, char> sixth(std::make_pair(30, 'c'));   // from pair / move

  std::cout << "sixth contains: " << std::get<0>(sixth);
  std::cout << " and " << std::get<1>(sixth) << '\n';

  // operator=
  std::pair<int, char> mypair(0, ' ');

  std::tuple<int, char> a(10, 'x');
  std::tuple<long, char> b, c;

  b = a;                           // copy assignment
  c = std::make_tuple(100L, 'Y');  // move assignment
  a = c;                           // conversion assignment
  c = std::make_tuple(100, 'z');   // conversion / move assignment
  a = mypair;                      // from pair assignment
  a = std::make_pair(2, 'b');      // form pair /move assignment

  std::cout << "c contains: " << std::get<0>(c);
  std::cout << " and " << std::get<1>(c) << '\n';
  PRINT("%ld %c", std::get<0>(c), std::get<1>(c));
}

static void TupleTieTest(void) {
  base_con::PrintFlag("Tuple tie test");
  std::tuple<int, char, std::string> foo(1, 'a', "apple");

  int id{};
  char letter{};
  std::string word{};

  std::tie(id, letter, word) = foo;
  PRINT("id %d, letter %c, word %s", id, letter, word.data());

  PRINT(
      "std::get<0>(foo) %d, std::get<1>(foo) %c, std::get<2>(foo) %s", std::get<0>(foo), std::get<1>(foo),
      std::get<2>(foo).data());

  std::get<0>(foo) = 2;
  PRINT(
      "std::get<0>(foo) %d, std::get<1>(foo) %c, std::get<2>(foo) %s", std::get<0>(foo), std::get<1>(foo),
      std::get<2>(foo).data());

  auto bar = std::make_tuple(14, 'y', "yellow");
  // if auto , save "yellow" to char *
  PRINT(
      "std::get<0>(bar) %d, std::get<1>(bar) %c, std::get<2>(bar) %s", std::get<0>(bar), std::get<1>(bar),
      std::get<2>(bar));
}

int main(int argc, char *argv[]) {
  try {
    ArraySizeTest();
    ArrayAssertTest();
    TupleConstructTest();
    TupleTieTest();
  } catch (std::exception &err) {
    std::cout << "exception:" << err.what() << std::endl;
  } catch (...) {
    std::cout << "exception but do nothing" << std::endl;  // do nothing
    throw;                                                 //重新抛出
  }
  return 0;
}
