

#include <getopt.h>
#include <pthread.h>
#include <stdint.h>

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <set>
#include <string>
#include <thread>
#include <vector>

#include "base_context.h"
#include "base_value.h"
#include "com_interface.h"
#include "common_log.h"
#include "log_printf.h"
#include "sys_terminal.h"

#define STR_FLAG "main"

int32_t main(int32_t argc, char** argv) {
  sys::Terminal trm{};
  std::string dev{"/dev/ttyUSB0"};
  int32_t ret = trm.Open(dev);
  if (ret != kSuccess) {
    PRINT("open %s failed", dev.data());
    return kFailed;
  }
  ret = trm.SelfTest();
}
