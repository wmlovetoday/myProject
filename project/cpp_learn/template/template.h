#ifndef EXM_CPPTEST_TMP
#define EXM_CPPTEST_TMP

#include <cstdint>
#include "log_printf.h"

namespace wm {
void Swap(int32_t &a, int32_t &b);
template <typename T>
void Swap(T &a, T &b);
template <typename TC, typename TI>
class Test {
 public:
  Test() { PRINT("This is constructor !"); }
  TC GetIns(TI ins) {
    TC ret{};
    ret.push_back(ins);
    return ret;
  }
};
}  // namespace wm

#endif