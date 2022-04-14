#ifndef INC_SYS_SYS_TH_
#define INC_SYS_SYS_TH_
#include <cstdint>
#include <functional>
#include <thread>

#include "log_printf.h"

namespace sys {
using ThRunFunc = std::function<int32_t(const bool &run, const bool &start, void *args)>;
class Th {
 public:
  Th() = default;
  int32_t Regist(ThRunFunc &run_func, const void *run_args);
  int32_t Remove() noexcept;
  /** start call back func*/
  inline void StartCallback() noexcept { start_run_ = true; }
  inline void StopCallback() noexcept { start_run_ = false; }
  ~Th();

 private:
  bool start_run_{false};
  bool init_{false};
  bool running_{false};
  std::thread th_{};
};

}  // namespace sys

#endif