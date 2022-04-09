#include <getopt.h>
#include <pthread.h>
#include <stdint.h>

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "base_context.h"
#include "com_interface.h"
#include "common_log.h"
#include "log_printf.h"

int main(int argc, char* argv[]) {
  PTS_INFO("i am another log {}", 90);
  zlog_category_t* usr_handle = pts::com_log->GetCategory("my_cat", "mylog.txt", 10, 10);
  PTS_U_INFO(usr_handle, "user log {}", 20);
  PTS_INFO("i am another log {}", 100);

  PTS_INFO("i am another log {0: }", 100);  // method of space
  PTS_INFO("i am another log {1}", 100, 200);
  PTS_INFO("{:<30}", "left aligned");      // fixed width
  PTS_INFO("{:<{}}", "left aligned", 30);  // dynamic width
  PTS_INFO("{:>30}", "right aligned");
  PTS_INFO("{:^30}", "centered");
  PTS_INFO("{:*^30}", "centered");

  PTS_INFO("{:.2f}", 23.14159265);
  PTS_INFO("{:.{}f}", 123.14, 1);

  PTS_INFO("{:0x} {:0x}", 5, 8);
  PTS_INFO("{:0x}", 10);                          // a
  PTS_INFO("{2:0x},{0:0x}", 10, 11, 12, 12, 14);  // a
  PTS_INFO("{:#06x}", 10);                        // 0x000a
  PTS_INFO("int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);

  /** nothing,  show only minus*/
  /** + show always, both positive and　minus*/
  /** space, show a space for positive numbers*/
  /**　-，show only the minus */

  PTS_INFO("{:+f}; {:+f}", 3.14, -3.14);  //+3.140000; -3.140000
  PTS_INFO("{: f}; {: f}", 3.14, -3.14);  // 3.140000; -3.140000
  PTS_INFO("{:-f}; {:-f}", 3.14, -3.14);  // 3.140000; -3.140000
  PTS_INFO("{:f}; {:f}", 3.14, -3.14);    // 3.140000; -3.140000

  /**new line*/
  PTS_INFO("{:f}; {} {:f}", 3.14, "\n", -3.14);

  std::string tmp{std::string("hello") + std::string("\n") + "world"};
  tmp += " !";
  PTS_INFO("{}", tmp);

  std::string sss = fmt::format("{:14.8f}", 123456.56789);
  PTS_INFO("{}", sss);
  std::string dir{"./test/tt/"};
  common::CreateDir(dir.data(), 0777);
  return 0;
}
