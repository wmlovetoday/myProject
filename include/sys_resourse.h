
#ifndef INC_SYS_RESOURCES_
#define INC_SYS_RESOURCES_

#include <iostream>
#include <string>

namespace sys_res {

struct SysCpu {
  float total;
  float usr;
  float sys;
};

struct SysMem {
  float me;
};
class SysRes {
public:
  SysRes() = default;
  struct SysCpu GetSysCpu();
  struct SysCpu GetProcessCpu();
  struct SysMem GetMem();
  ~SysRes(){};

private:
};

} // namespace sys_res
#endif