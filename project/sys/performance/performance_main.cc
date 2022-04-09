

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
#include "com_interface.h"
#include "common_log.h"
#include "log_printf.h"
#include "sys_res.h"

#define STR_FLAG "main"

static void Resource(const std::vector<uint32_t>& pid) {
  sys::Res Res{};
  sys::cpu_usage loc_cpu{};
  sys::pro_cpu_usage cpu{};
  sys::statm mem{};
  int32_t ret = 0;
  while (true) {
    for (auto i : pid) {
      std::this_thread::sleep_for(std::chrono::seconds(1));
      ret = Res.GetProgressCpuUsage(std::to_string(i), cpu);
      if (ret == 0) {
        PRINT("usr %2f; sys %2f; total %2f", cpu.usr, cpu.sys, cpu.total);
      }
      ret = Res.GetProgressMemUsage(std::to_string(i), mem);
      if (ret == 0) {
        PRINT("mem %.2f MB", mem.vm_rss);
      }
    }
    Res.GetCpuUsage(loc_cpu);
    PRINT("local cpu %.2f %.2f %.2f", loc_cpu.one_min, loc_cpu.five_min, loc_cpu.fifteen_min);
  }
}

int32_t main(int32_t argc, char** argv) {
  int32_t ret = 0;

  std::string pid_name{argv[1]};
  std::vector<uint32_t> pid{};
  ret = com::GetPid(pid_name, pid);
  if (ret == 0) {
    for (auto i : pid) {
      PRINT("pidof %s = %d", pid_name.data(), i);
    }
  } else {
    PRINT("pidof perf failed");
  }
  std::thread th = std::thread([&]() -> void { Resource(pid); });
  th.join();
  // pid_t pidd = getpid();
}
