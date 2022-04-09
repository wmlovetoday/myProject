#ifndef INC_SYS_RESOURSE_
#define INC_SYS_RESOURSE_

#include <unistd.h>

#include <cassert>
#include <cerrno>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <mutex>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "com_interface.h"

namespace sys {

constexpr int32_t kSysSuccess = 0;
constexpr int32_t kSysFailed = -1;
constexpr int32_t kSysTimeout = -2;
constexpr int32_t kSysNotSupport = -3;
constexpr int32_t kSysInvalidArgs = -4;

struct StatData {
  void parse(const std::string& content) {
    size_t rp = content.rfind(')');
    std::istringstream iss(content.data() + rp + 1);

    //            0    1    2    3     4    5       6   7 8 9  11  13   15
    // 3770 (cat) R 3718 3770 3718 34818 3770 4202496 214 0 0 0 0 0 0 0 20
    // 16  18     19      20 21                   22      23      24 25
    //  0 1 0 298215 5750784 81 18446744073709551615 4194304 4242836
    //  140736345340592
    //              26
    // 140736066274232 140575670169216 0 0 0 0 0 0 0 17 0 0 0 0 0 0

    iss >> state;
    iss >> ppid >> pgrp >> session >> tty_nr >> tpgid >> flags;
    iss >> minflt >> cminflt >> majflt >> cmajflt;
    iss >> utime >> stime >> cutime >> cstime;
    iss >> priority >> nice >> num_threads >> itrealvalue >> starttime;
  }
  char state;
  int ppid;
  int pgrp;
  int session;
  int tty_nr;
  int tpgid;
  int flags;

  long minflt;
  long cminflt;
  long majflt;
  long cmajflt;

  long utime;
  long stime;
  long cutime;
  long cstime;

  long priority;
  long nice;
  long num_threads;
  long itrealvalue;
  long starttime;
};

struct pro_cpu_usage {
  float total;
  float usr;
  float sys;
};

struct cpu_usage {
  float one_min;
  float five_min;
  float fifteen_min;
};

struct statm {
  float program_size;
  float vm_rss;
  float shared;
};

class Res {
 public:
  Res();
  int32_t GetProgressCpuUsage(const std::string& pid, struct pro_cpu_usage& usg);
  int32_t GetCpuUsage(struct cpu_usage& usg);
  int32_t GetProgressMemUsage(const std::string& pid, struct statm& mb);
  ~Res() = default;

 private:
  bool init_{false};
  int32_t pagesize_{};
  int32_t clockTicks_{};
  int64_t old_usr_ticks_{};
  int64_t old_sys_ticks_{};
  int64_t new_usr_ticks_{};
  int64_t new_sys_ticks_{};

  int64_t old_ms_{};
  int64_t new_ms_{};

  int64_t GetMs() {
    timespec old_tv{};
    int32_t ret = clock_gettime(CLOCK_REALTIME, &old_tv);
    if (ret == 0) {
      int64_t ms = old_tv.tv_sec * 1000 + old_tv.tv_nsec / 1000000;
      return ms;
    }
    return kSysFailed;
  }
};

}  // namespace sys
#endif