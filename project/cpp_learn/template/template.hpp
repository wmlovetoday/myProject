#include "template.h"

namespace wm {
void Swap(int32_t &a, int32_t &b) {
  int32_t t = a;
  a = b;
  b = t;
  PRINT("void Swap(int32_t &a, int32_t &b)");
}
template <typename T>
void Swap(T &a, T &b) {
  T t = a;
  a = b;
  b = t;
  PRINT("void Swap(T &a, T &b)");
}
}  // namespace wm