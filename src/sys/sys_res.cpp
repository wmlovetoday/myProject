#include <cstdint>
#include <stdexcept>
#include <string>

#include "sys_res.h"

namespace sys {
Res::Res() {
  clockTicks_ = static_cast<int>(sysconf(_SC_CLK_TCK));
  pagesize_ = com::GetPagesize();
  int64_t ms = GetMs();
  if (ms > 0) {
    old_ms_ = ms;
  }
  if ((old_ms_ <= 0) && (clockTicks_ <= 0) && (pagesize_ <= 0)) {
    throw(std::runtime_error{"update time failed"});
  }
}

int32_t Res::GetProgressCpuUsage(const std::string& pid, struct pro_cpu_usage& usg) {
  std::string cmd{"cat /proc/" + pid + "/stat"};
  std::string ack{};
  StatData sta{};
  int64_t ms = GetMs();
  int32_t ret = com::ReadCmd(cmd.data(), ack, 2);
  if ((ret == kSysSuccess) && (ms > 0) && (!ack.empty())) {
    sta.parse(ack);
    new_ms_ = ms;
    if (init_) {
      float period_sec = (new_ms_ - old_ms_) / 1000.0;
      new_usr_ticks_ = sta.cutime - old_usr_ticks_;
      new_sys_ticks_ = sta.cstime - old_sys_ticks_;
      int64_t total_ticks_ = new_usr_ticks_ + new_sys_ticks_;
      if (period_sec && clockTicks_) {
        usg.total = total_ticks_ / (period_sec * clockTicks_);
        usg.usr = new_usr_ticks_ / (period_sec * clockTicks_);
        usg.sys = new_sys_ticks_ / (period_sec * clockTicks_);
        return kSysSuccess;
      }
      PRINT("tic = %d, period = %f", clockTicks_, period_sec);
      return kSysFailed;
    }  // first
    old_ms_ = ms;
    old_usr_ticks_ = sta.cutime;
    old_sys_ticks_ = sta.cstime;
    init_ = true;
    usg.total = 0.0;
    usg.usr = 0.0;
    usg.sys = 0.0;
    return kSysSuccess;
  }
  PRINT("get sys status failed");
  return kSysFailed;
}

int32_t Res::GetCpuUsage(struct cpu_usage& usg) {
  float usage[3] = {0.0, 0.0, 0.0};
  int index = 0;
  std::string cmd{"uptime"};
  std::string begin{"load average: "};
  std::string end{", "};
  /* 18:08:32 up 8 days,  9:04,  1 user,  load average: 0.21, 0.28, 0.37 */
  std::string ack{};
  int32_t ret = com::ReadCmd(cmd.data(), ack, 2);
  if ((ret == kSysSuccess) && (!ack.empty())) {
    size_t pos = ack.rfind(begin);
    if (pos != std::string::npos) {
      // PRINT("get : %s", ack.data() + pos + begin.size());
      char* pch = strtok(const_cast<char*>(ack.data() + pos + begin.size()), end.data());
      while (pch != NULL) {
        // PRINT("val : %s", pch);
        usage[index++] = std::stof(pch);
        pch = strtok(NULL, end.data());
      }
    } else {
      PRINT("not find /");
    }
    usg.one_min = usage[0];
    usg.five_min = usage[1];
    usg.fifteen_min = usage[2];
    return kSysSuccess;
  }
  PRINT("get sys status failed");
  return kSysFailed;
}

int32_t Res::GetProgressMemUsage(const std::string& pid, struct statm& mb) {
  std::string mem_cmd{"cat /proc/" + pid + "/statm"};
  std::string mem_ack{};
  int32_t ret = com::ReadCmd(mem_cmd.data(), mem_ack, 2);
  if ((ret == kSysSuccess) && (!mem_ack.empty())) {
    uint32_t program_size = 0;
    uint32_t vm_rss = 0;
    uint32_t shared = 0;
    std::stringstream ss{mem_ack};
    ss >> program_size >> vm_rss >> shared;
    mb.program_size = ((program_size * pagesize_) / 1024) / 1024.0;
    mb.vm_rss = ((vm_rss * pagesize_) / 1024) / 1024.0;
    mb.shared = ((shared * pagesize_) / 1024) / 1024.0;
    return kSysSuccess;
  }
  PRINT("get sys status failed");
  return kSysFailed;
}
}  // namespace sys