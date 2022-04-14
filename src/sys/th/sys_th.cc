#include "sys_th.h"

namespace sys {

int32_t Th::Regist(ThRunFunc &run_func, const void *run_args) {
  if (init_) {
    return -1;
  }
  running_ = true;
  th_ = std::thread([&]() -> void { run_func(running_, start_run_, const_cast<void *>(run_args)); });
  if (th_.joinable()) {
    init_ = true;
    PRINT("thread create success");
    return (0);
  } else {
    running_ = false;
    return -1;
  }
}

int32_t Th::Remove() noexcept {
  if (init_) {
    start_run_ = false;
    init_ = false;
    running_ = false;
    std::this_thread::sleep_for(std::chrono::seconds(1));  // wait ThRunFunc exit.
    if (th_.joinable()) {
      pthread_cancel(th_.native_handle());
      th_.join();
    }
  }
  PRINT("remove thread");
  return (0);
}

Th::~Th() {
  if (init_) {
    Remove();
  }
  init_ = false;
}
}  // namespace sys