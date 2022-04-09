#include <cassert>  //assert
#include <cstdint>
#include <exception>  //static_assert
#include <iostream>
#include <regex>
#include <string>

#include "base_context.h"
#include "common_log.h"
#include "log_printf.h"

/** 功能
  检查一个串是否包含某种形式的子串； regex_search
  将匹配的子串替换；  regex_match
  从某个串中取出符合条件的子串。 regex_match

  先定义一个 std::regex r{};
  regex_match(src, output, r)
*/
/**
  http://help.locoy.com/Document/Learn_Regex_For_30_Minutes.htm

特殊字符
  \    如果不想让这些特殊字符生效，那必须转义它
  ^    匹配输入字符串的开始位置，除非在方括号表达式中使用，此时它表示不接受该字符集合。
  $	   匹配输入字符串的结尾位置。
  (,)	 标记一个子表达式的开始和结束位置。子表达式可以获取供以后使用。
  *	   匹配前面的子表达式零次或多次。
  +	   匹配前面的子表达式一次或多次。
  .	   匹配除换行符 \n 之外的任何单字符。
  [	   标记一个中括号表达式的开始。
  ?	   匹配前面的子表达式零次或一次，或指明一个非贪婪限定符。
  \	  将下一个字符标记为或特殊字符、或原义字符、或向后引用、或八进制转义符。例如， n 匹配字符 n。\n 匹配换行符。序列
       \\ 匹配 '\' 字符，而 \( 则匹配 '(' 字符。
  {	  标记限定符表达式的开始。
  |    指明两项之间的一个选择。
*/

/**
  *	匹配前面的子表达式零次或多次。例如，foo* 能匹配 fo 以及 foooo。* 等价于{0,}。
  +	匹配前面的子表达式一次或多次。例如，foo+ 能匹配 foo 以及 foooo，但不能匹配 fo。+ 等价于 {1,}。
  ?	匹配前面的子表达式零次或一次。例如，Your(s)? 可以匹配 Your 或 Yours 中的Your 。? 等价于 {0,1}。

  {n}	n 是一个非负整数。匹配确定的 n 次。例如，o{2} 不能匹配 for 中的 o，但是能匹配 foo 中的两个 o。
  {n,}	n 是一个非负整数。至少匹配 n 次。例如，o{2,} 不能匹配 for 中的 o，但能匹配 foooooo 中的所有 o。o{1,} 等价于
  o+。o{0,} 则等价于 o*。 {n,m}	m 和 n 均为非负整数，其中 n 小于等于 m。最少匹配 n 次且最多匹配 m 次。例如，o{1,3}
  将匹配 foooooo 中的前三个 o。o{0,1} 等价于 o?。注意，在逗号和两个数之间不能有空格。
*/

/**
  .*       任意数量的不包含换行的字符
  \bhi\b   精确找到 hi,HI,Hi,hI ，否则可能找到　him,history,high
  \bhi\b.*\bLucy\b  找的是hi后面不远处跟着一个Lucy

  \d       匹配一位数字
  \d{8,5}  匹配八或者五位数字
  \d+      任意位数字

  \s       匹配任意的空白符，包括空格，制表符(Tab)，换行符，中文全角空格等。
  \w       匹配字母或数字或下划线或汉字等。

  \d{5,12}$
*/

/**
  [0-9] 代表的含意与\d就是完全一致的：一位数字；
  [a-z0-9A-Z_] 也完全等同于\w
  [aeiou] 就匹配任何一个英文元音字母，
  [.?!]匹配标点符号(.或?或!)。
*/

// \(?0\d{2}[) -]?\d{8} 可以过滤出 (010)88886666，或022-22334455，或02912345678
// 转义字符\(,它能出现0次或1次(?),然后是一个0，后面跟着2个数字(\d{2})，
// 然后是)或-或空格中的一个，它出现1次或不出现(?)，最后是8个数字(\d{8})

// 010)12345678或(022-87654321这样的“不正确”的格式 也符合上面的格式．

/**
  |  用来表示匹配任意一种
  (\d{1,3}\.){3}　定义子表达式重复的次数，(小括号内是子表达式，之前的规则都是单个字符的重复次数)
  \W	匹配任意不是字母，数字，下划线，汉字的字符
  \S	匹配任意不是空白符的字符
  \D	匹配任意非数字的字符
  \B	匹配不是单词开头或结束的位置
  [^x]	匹配除了x以外的任意字符
  [^aeiou]	匹配除了aeiou这几个字母以外的任意字符
*/

/**
C-strings (1)
template <class charT, class traits>
  bool regex_match (const charT* s, const basic_regex<charT,traits>& rgx,
          regex_constants::match_flag_type flags = regex_constants::match_default);
strings (2)
template <class ST, class SA, char charT, class traits>
  bool regex_match (const basic_string<charT,ST,SA>& s,
          const basic_regex<charT,traits>& rgx,
          regex_constants::match_flag_type flags = regex_constants::match_default);
ranges (3)
template <class BidirectionalIterator, class charT, class traits>
  bool regex_match (BidirectionalIterator first, BidirectionalIterator last,
          const basic_regex<charT,traits>& rgx,
          regex_constants::match_flag_type flags = regex_constants::match_default);
with match_results (4,5,6)
template <class charT, class Alloc, class traits>
  bool regex_match (const charT* s, match_results<const charT*, Alloc>& m,
          const basic_regex<charT,traits>& rgx,
          regex_constants::match_flag_type flags = regex_constants::match_default);
template <class ST, class SA, class Alloc, class charT, class traits>
  bool regex_match (const basic_string<charT,ST,SA>& s,
          match_results<typename basic_string<charT,ST,SA>::const_iterator,Alloc>& m,
          const basic_regex<charT,traits>& rgx,
          regex_constants::match_flag_type flags = regex_constants::match_default);
template <class BidirectionalIterator, class Alloc, class charT, class traits>
  bool regex_match (BidirectionalIterator first, BidirectionalIterator last,
          match_results<BidirectionalIterator, Alloc>& m,
          const basic_regex<charT,traits>& rgx,
          regex_constants::match_flag_type flags = regex_constants::match_default);
*/

static void RegexComapreTest(const std::string& src, const std::string& com) {
  base_con::PrintFlag("Regex Comapre test");
  if (!src.empty()) {
    std::regex tmp{com.data()};
    bool match = std::regex_match(src, tmp);
    if (match) {
      PRINT("match success : src: %s  comapre: %s", src.data(), com.data());
    } else {
      PRINT("match failed : src: %s  comapre: %s", src.data(), com.data());
    }
  } else {
    PRINT("arguments illegal");
  }
}
/**
C-strings (1)
template <class charT, class traits>
  bool regex_search (const charT* s, const basic_regex<charT,traits>& rgx,
          regex_constants::match_flag_type flags = regex_constants::match_default);
strings (2)
template <class ST, class SA, char charT, class traits>
  bool regex_search (const basic_string<charT,ST,SA>& s,
          const basic_regex<charT,traits>& rgx,
          regex_constants::match_flag_type flags = regex_constants::match_default);
ranges (3)
template <class BidirectionalIterator, class charT, class traits>
  bool regex_search (BidirectionalIterator first, BidirectionalIterator last,
          const basic_regex<charT,traits>& rgx,
          regex_constants::match_flag_type flags = regex_constants::match_default);
with match_results (4,5,6)
template <class charT, class Alloc, class traits>
  bool regex_search (const charT* s, match_results<const charT*, Alloc>& m,
          const basic_regex<charT,traits>& rgx,
          regex_constants::match_flag_type flags = regex_constants::match_default);
template <class ST, class SA, class Alloc, class charT, class traits>
  bool regex_search (const basic_string<charT,ST,SA>& s,
          match_results<typename basic_string<charT,ST,SA>::const_iterator,Alloc>& m,
          const basic_regex<charT,traits>& rgx,
          regex_constants::match_flag_type flags = regex_constants::match_default);
template <class BidirectionalIterator, class Alloc, class charT, class traits>
  bool regex_search (BidirectionalIterator first, BidirectionalIterator last,
          match_results<BidirectionalIterator, Alloc>& m,
          const basic_regex<charT,traits>& rgx,
          regex_constants::match_flag_type flags = regex_constants::match_default);
*/
static void RegexSearchTest(const std::string& src, const std::string& com) {
  base_con::PrintFlag("Regex search test");
  if (!src.empty()) {
    std::regex tmp{com.data()};
    std::smatch result;
    bool match = std::regex_search(src, result, tmp);
    if (match) {
      PRINT("search success : src: %s  comapre: %s", src.data(), com.data());
      for (auto x : result) std::cout << x << " ";
    } else {
      PRINT("search failed : src: %s  comapre: %s", src.data(), com.data());
    }
  } else {
    PRINT("arguments illegal");
  }
}

int main(int argc, char* argv[]) {
  try {
    std::string uptime{"11:30:55 up 8 days,  2:27,  1 user,  load average: 0.42, 0.43, 0.41"};
    std::string src{"load average"};
    std::string com{R"(.*)"};
    std::string com1{R"(\w*\s*\w*)"};
    std::string com2{R"(\w+\s+\w+)"};
    std::string com3{R"((\w+)\s*(\w+))"};

    std::string cm{R"(load average:\s+\d*.\d*,)"};
    std::string cpu_load_e(R"(load average: (\d+.*\d+), (\d+.*\d+), (\d+.*\d+))");
    std::string cpu_load_ee(R"(load average: (\d+.*\d+),)");

    RegexComapreTest(src, com);
    RegexComapreTest(src, com1);
    RegexComapreTest(src, com2);
    RegexComapreTest(src, com3);
    RegexComapreTest(uptime, cm);

    RegexSearchTest(uptime, cm);
    RegexSearchTest(uptime, cpu_load_e);
    RegexSearchTest(uptime, cpu_load_ee);

    std::regex begin{R"(load average:.*)"};
    std::regex value{R"((\b\d+.*\d+,))"};
    const std::regex cpu_load_e(R"(load average: (\d+.*\d+), (\d+.*\d+), (\d+.*\d+))");
    std::smatch match;
    bool ser = std::regex_search(uptime, match, begin);
    if (ser) {
      PRINT("FIRST : ");
      for (auto x : match) std::cout << x << " ";
      std::string sub_uptime = match.suffix().str();
      PRINT("%s", sub_uptime.data());
      ser = std::regex_search(sub_uptime, match, value);
      if (ser) {
        PRINT("val : ");
        PRINT("%s", match.suffix().str());
      }
    }

  } catch (std::exception& err) {
    std::cout << "exception:" << err.what() << std::endl;
  } catch (...) {
    std::cout << "exception but do nothing" << std::endl;  // do nothing
    throw;                                                 //重新抛出
  }
  return 0;
}
