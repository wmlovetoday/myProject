#ifndef INC_SYS_SYS_DEFS_
#define INC_SYS_SYS_DEFS_

#include <unistd.h>

namespace sys {

constexpr int32_t kSysSuccess = 0;
constexpr int32_t kSysFailed = -1;
constexpr int32_t kSysTimeout = -2;
constexpr int32_t kSysNotSupport = -3;
constexpr int32_t kSysInvalidArgs = -4;

}  // namespace sys
#endif