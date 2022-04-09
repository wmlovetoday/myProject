#include <cassert>  //assert
#include <cstdint>
#include <exception>  //static_assert
#include <iostream>
#include <string>

#include <getopt.h>
#include <unistd.h>

#include "base_context.h"
#include "common_log.h"
#include "log_printf.h"

/**
int getopt(int argc, char *const argv[], const char *optstring);
extern char *optarg;
extern int optind, opterr, optopt;

#include <getopt.h>
int getopt_long(int argc, char *const argv[], const char *optstring, const struct option *longopts, int *longindex);
*/

static void OptionTest(int argc, char **argv, char *flag) {
  base_con::PrintFlag("Option test");
  int32_t ret = 0;
  while ((ret = getopt(argc, argv, flag)) != -1) {
    switch (ret) {
      case 'a':
        PRINT("opt is a, oprarg is: %s", optarg);
        break;
      case 'b':
        PRINT("opt is b, oprarg is: %s", optarg);
        break;
      case 'c':
        PRINT("opt is c, oprarg is: %s", optarg);
        break;
      case 'h':
        PRINT("you can input : %s", flag);
        break;
      case '?':
        PRINT("error optopt: %c", optopt);
        PRINT("error opterr: %d", opterr);
        break;
    }
  }
}

/**
struct option {
  const char *name; //name表示的是长参数名
  int has_arg；  //has_arg有3个值，no_argument(或者是0)，表示该参数后面不跟参数值
                // required_argument(或者是1),表示该参数后面一定要跟个参数值
                // optional_argument(或者是2),表示该参数后面可以跟，也可以不跟参数值
  int *flag;    //用来决定，getopt_long()的返回值到底是什么。如果flag是null，则函数会返回与该项option匹配的val值
  int val;      //和flag联合决定返回值
}
*/
option long_opts[] = {
    {"server", required_argument, nullptr, 's'},
    {"client", required_argument, nullptr, 'c'},
    {"port", required_argument, nullptr, 'p'},
    {"help", no_argument, nullptr, 'h'},
    {nullptr, no_argument, nullptr, 0}};

static void OptionLongTest(int argc, char **argv, char *flag, option *opts) {
  base_con::PrintFlag("Option long test");
  int32_t ret = 0;

  while (true) {
    ret = getopt_long(argc, argv, flag, opts, nullptr);
    PRINT("ret = %d", ret);
    if (ret < 0) break;
    switch (ret) {
      case 's':
        PRINT("opt is s, oprarg is: %s", optarg);
        break;
      case 'c':
        PRINT("opt is c, oprarg is: %s", optarg);
        break;
      case 'p':
        PRINT("opt is p, oprarg is: %s", optarg);
        break;
      case 'h':
        PRINT("you can input : %s", flag);
        break;
      case '?':
        PRINT("error optopt: %c", optopt);
        PRINT("error opterr: %d", opterr);
        break;
      default:
        break;
    }
  }
}

int main(int argc, char *argv[]) {
  try {
    char *optstring = "abc:s:c:p:h";  // 有三个选项-abc，其中c选项后有冒号，所以后面必须有参数
    PTS_INFO("you can input : {}", optstring);
    // OptionTest(argc, argv, optstring);
    OptionLongTest(argc, argv, optstring, long_opts);

  } catch (std::exception &err) {
    std::cout << "exception:" << err.what() << std::endl;
  } catch (...) {
    std::cout << "exception but do nothing" << std::endl;  // do nothing
    throw;                                                 //重新抛出
  }
  return 0;
}
