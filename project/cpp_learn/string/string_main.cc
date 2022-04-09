#include <cassert>  //assert
#include <condition_variable>
#include <cstddef>
#include <exception>  //static_assert
#include <iostream>
#include <iterator>
#include <mutex>  // std::mutex, std::lock_guard
#include <string>

#include "base_context.h"
#include "com_interface.h"
#include "common_log.h"
#include "log_printf.h"

template <typename TC>
static void Attributes(TC &myContainer) {
  if (!myContainer.empty()) {
    PRINT("myContainer.data()= %s", myContainer.data());
    PRINT("container size %ld", myContainer.size());
    PRINT("myContainer.capacity() = %ld", myContainer.capacity());
    PRINT("container max_size %ld", myContainer.max_size());
  } else {
    PRINT("container is empty");
  }
}

/**
  basic_string (const basic_string& str); //copy
  basic_string (const basic_string& str, size_type pos, size_type len = npos); // sub string
  basic_string (const charT* s);  //c
  basic_string (const charT* s, size_type n);
  basic_string (size_type n, charT c);
  template <class InputIterator>
    basic_string  (InputIterator first, InputIterator last );
  basic_string (initializer_list<charT> il);
basic_string (basic_string&& str) noexcept;
*/
void StringConstructorTest() {
  base_con::PrintFlag("String Constructor test");

  std::string s0("Initial string");
  std::string s2(s0);
  std::string s3(s0, 8, 3);
  std::string t8{s0, 1, std::string::npos};
  std::string s4("A character sequence");
  std::string s5("A character sequence", 6);
  std::string s6(10, 'x');
  std::string s7a(10, 42);
  std::string s7b(s0.begin(), s0.begin() + 7);
  Attributes(s5);
}

void StringOperationTest() {
  base_con::PrintFlag("String Operation test");

  std::string s0("Initial string");
  std::string t1{s0, 1, std::string::npos};
  t1 += "-a";
  t1 += 'A';
  t1 += std::string{"-bB"};
  Attributes(t1);

  std::string firstlevel("com");
  std::string secondlevel("cplusplus");
  std::string scheme("http://");
  std::string hostname;
  std::string url;
  hostname = "www." + secondlevel + '.' + firstlevel;
  url = scheme + hostname;
  Attributes(url);
}

// string to_string (int val);
// string to_string (long val);
// string to_string (long long val);
// string to_string (unsigned val);
// string to_string (unsigned long val);
// string to_string (unsigned long long val);
// string to_string (float val);
// string to_string (double val);
// string to_string (long double val);

// int stoi (const string&  str, size_t* idx = 0, int base = 10);
// base = 0 , for auto.

void StringConvertItoSTest() {
  base_con::PrintFlag("String Convert ItoS test");

  std::string s0{std::to_string(123.456)};
  PRINT("to_string(123.456) = %s", s0.data());
}

void StringConvertStoITest() {
  base_con::PrintFlag("String Convert StoI test");
  std::string str_dec = "2001 A Space 2 Odyssey";
  std::string::size_type sz;  // alias of size_t

  // int i_dec = std::stoi(str_dec, &sz);
  int i_dec = std::stoi(str_dec);
  PRINT("i_dec = %d", i_dec);

  std::string str_hex = "40c3";
  int i_hex = std::stoi(str_hex);
  PRINT("i_hex = %d", i_hex);  // 40

  std::string max{"12345678912345"};
  i_hex = std::stoi(str_hex, nullptr, 16);
  PRINT("i_hex = %d", i_hex);  // 40
  // unsigned long u = std::stoi(max, nullptr, 0); //exception:stoi
  unsigned long ul = std::stoul(max, nullptr, 0);
  PRINT("ul = %ld", ul);

  char c = 'w';
  std::cout << "isspace('w')=" << isspace(c) << std::endl;  //是空格？
  std::cout << "isalpha('w')=" << isalpha(c) << std::endl;  //是字符？
  std::cout << "isdigit('w')=" << isdigit(c) << std::endl;  //是数字？
  std::cout << "isupper('w')=" << isupper(c) << std::endl;  //大写?
  std::cout << "islower('w')=" << islower(c) << std::endl;  //小写?
  std::cout << "isalnum('w')=" << isalnum(c) << std::endl;
  std::cout << "toupper('w')=" << toupper(c) << std::endl;  //转大写
  std::cout << "tolower('w')=" << tolower(c) << std::endl;  //转小写
}

// void resize (size_t n);
// void resize (size_t n, char c);

void StringCapacityTest() {
  base_con::PrintFlag("String Capacity test");

  std::string str2{};
  PRINT("std::string str2{};");
  Attributes(str2);

  str2.resize(20, 65);
  base_con::PrintFlag("");
  PRINT("str2.resize(5,65);");
  Attributes(str2);

  // string &erase(size_t pos = 0, size_t len = npos);
  // iterator erase(const_iterator p);
  // iterator erase(const_iterator first, const_iterator last); begin()+5 end()-5
  str2.erase(0, 10);
  base_con::PrintFlag("");
  PRINT("str2.erase(0, 10)");
  Attributes(str2);

  str2.clear();
  base_con::PrintFlag("");
  PRINT("str2.clear();");
  Attributes(str2);

  // std::string str3{};
  // for (int i = 65; i < 70; i++) str3.push_back(i);
  // PRINT("str3= %s", str3.c_str());
}
void StringCapacity_Test() {
  base_con::PrintFlag("String assign test");
  PRINT("*************** %s ****************", "basic string assign test");
  std::string tmp{};
  tmp.assign("temp sting");
  PRINT("%s size %ld", tmp.data(), tmp.size());
  tmp.assign("temp add");
  PRINT("%s size %ld", tmp.data(), tmp.size());
  tmp.append("wang");
  PRINT("%s size %ld", tmp.data(), tmp.size());
}

/**
  string& append (const string& str);
  string& append (const string& str, size_t subpos, size_t sublen);
  string& append (const char* s);
  string& append (const char* s, size_t n);
  string& append (size_t n, char c);
  template <class InputIterator>
    string& append (InputIterator first, InputIterator last);
  string& append (initializer_list<char> il);
*/
/**
  string& insert (size_t pos, const string& str);
  string& insert (size_t pos, const string& str, size_t subpos, size_t sublen);
  string& insert (size_t pos, const char* s);
  string& insert (size_t pos, const char* s, size_t n);
  string& insert (size_t pos,   size_t n, char c);
  iterator insert (const_iterator p, size_t n, char c);
  iterator insert (const_iterator p, char c);
  template <class InputIterator>
  iterator insert (iterator p, InputIterator first, InputIterator last);
  string& insert (const_iterator p, initializer_list<char> il);
  assign,same with insert,except pos.
 */
static void StringAppendInsertTest(void) {
  base_con::PrintFlag("String Append Insert Test");
  std::string str1{};
  std::string str2{};
  str1.append("i am a ");
  str1.append(10, 'g');
  PRINT("S1 = %s", str1.data());
  str2.append(str1, 0, 5);
  str2.append({'a', ' '});
  str2.append("boy");
  PRINT("str2= %s", str2.c_str());

  str1.clear();
  str1.insert(0, str2);
  str1.insert(str1.end(), 3, '!');
  PRINT("str1= %s", str1.c_str());
}
/**
  int compare (const string& str) const noexcept;
  int compare (size_t pos, size_t len, const string& str) const;
  int compare (size_t pos, size_t len, const string& str,
              size_t subpos, size_t sublen) const;
  int compare (const char* s) const;
  int compare (size_t pos, size_t len, const char* s) const;
  int compare (size_t pos, size_t len, const char* s, size_t n) const;
*/
static void StringCompareTest() {
  base_con::PrintFlag("String Compare Test");
  uint8_t str[] = {"hello"};
  std::string s1{"hello"};
  std::string s2{reinterpret_cast<char *>(str)};

  if (s1 == s2)
    PRINT("run s1 == s2,s1 equal s2");
  else
    PRINT("run s1 == s2,s1 not equal s2");

  if (s1.compare(s2) == 0)
    PRINT("run s1.compare(s2) ,s1 equal s2");
  else
    PRINT("run s1.compare(s2) ,s1 not equal s2");

  std::string s3{};
  if (s3 == s2)
    PRINT("run s3 == s2,s3 equal s2");
  else
    PRINT("run s3 == s2,s3 not equal s2");

  if (s3.compare(s2) == 0)
    PRINT("run s3.compare(s2) ,s3 equal s2");
  else
    PRINT("run s3.compare(s2) ,s3 not equal s2");
}

/**
  istream& getline (istream&  is, string& str, char delim);
  istream& getline (istream&& is, string& str, char delim);
  istream& getline (istream&  is, string& str);
  istream& getline (istream&& is, string& str);
*/
static void StringGetlineTest() {
  base_con::PrintFlag("String Getline Test");
  std::cout << "Please, enter your full name: ";
  std::string name{};
  std::getline(std::cin, name);
  std::cout << "Hello, " << name << "!\n";
}

/**
返回最后一次匹配的位置
  const char * strrchr ( const char * str, int character );
        char * strrchr (       char * str, int character );
返回第一次匹配的位置
  const char * strchr ( const char * str, int character );
        char * strchr (       char * str, int character );
  const void * memchr ( const void * ptr, int value, size_t num );
        void * memchr (       void * ptr, int value, size_t num );
*/
/**
  // 最后一次出现的位置
  size_t rfind (const string& str, size_t pos = npos) const noexcept;
  size_t rfind (const char* s, size_t pos = npos) const;
  size_t rfind (const char* s, size_t pos, size_t n) const;
  size_t rfind (char c, size_t pos = npos) const noexcept;

//返回第一次/最后一次匹配的位置
  find_first_of/find_last_of  same with rfind
*/
static void StringFindTest() {
  base_con::PrintFlag("String Find Test");
  std::string name{"/home/code/test.c"};
  // std::string name{"test.c"};
  size_t pos = name.rfind('/');
  if (pos != std::string::npos) {
    PRINT("remove path : %s", name.data() + pos + 1);
  } else {
    PRINT("not find /");
  }
}

/**
 * 在 str1 中找到第一次与 str2（单个字符部分或全部） 匹配位置
  size_t strcspn ( const char * str1, const char * str2 );

  str2 在 str1 中匹配的长度。 好像冻太好用
  size_t strspn ( const char * str1, const char * str2 );
*/
static void CharStringCompareTest() {
  base_con::PrintFlag("char String Compare Test");
  std::string name{"code_test.c"};  // /code/test.c
  std::string compare{"test.c"};
  size_t pos = strcspn(name.data(), compare.data());
  if (pos != std::string::npos) {
    PRINT("comapred contex : %s", name.data() + pos + 1);
  } else {
    PRINT("not find /");
  }

  size_t x = strspn(name.data(), compare.data());
  PRINT("comapred contex length : %ld", x);
}

/**
 * char * strtok ( char * str, const char * delimiters );
 */

static void CharStringSplitTest() {
  base_con::PrintFlag("char String split Test");
  char str[] = "- This, a sample string.";
  char *pch;
  PRINT("Splitting string \"%s\" into tokens:", str);
  pch = strtok(str, " ,.-");
  while (pch != NULL) {
    PRINT("%s", pch);
    pch = strtok(NULL, " ,.-");
  }
}

static void SelectTest() {
  base_con::PrintFlag("String Select Test");
  // std::string name{"/home/code/test.c"};
  std::string name{"test.c"};
  // const char *sel = strrchr(name.data(), '/') + 1; //如果并不知道是否含有 '/',必须提前判断，否则core
  const char *sel = strrchr(name.data(), '/') ? strrchr(name.data(), '/') + 1 : name.data();
  PRINT("remove path : %s", sel);

  char *pch;
  char str[] = "Example post";
  pch = (char *)memchr(str, 'p', strlen(str));
  PRINT("p is find at : %ld", pch - str + 1);

  std::string src{"i am a good boy"};
  std::string com{" "};
  std::vector<std::string> ret{};
  int32_t re = common::SpiltStr(src, com, ret);
  if (re == 0) {
    for (auto i : ret) {
      PRINT(" : %s", i.data());
    }
  }
}

int main(int argc, char *argv[]) {
  try {
    StringConstructorTest();
    StringOperationTest();
    StringConvertItoSTest();
    StringConvertStoITest();
    StringCapacityTest();
    StringAppendInsertTest();
    StringCompareTest();
    StringGetlineTest();
    StringFindTest();
    SelectTest();
    CharStringCompareTest();
    CharStringSplitTest();
  } catch (std::exception &err) {
    std::cout << "exception:" << err.what() << std::endl;
  } catch (...) {
    std::cout << "exception but do nothing" << std::endl;  // do nothing
    throw;                                                 //重新抛出
  }
  return 0;
}
