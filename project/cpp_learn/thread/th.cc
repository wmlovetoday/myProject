#include "th.h"

int32_t Th::Regist(Callback const &call_back) noexcept {
  if (!init_) {
    call_back_inc_ = call_back;
  } else {
    return -1;
  }
  running_ = true;
  acquire_ = std::thread([&]() -> void { AcquireThread(test_); });
  if (acquire_.joinable()) {
    init_ = true;
    PRINT("thread create success");
    return (0);
  } else {
    running_ = false;
    return -1;
  }
}
void Th::AcquireThread(uint8_t test) {
  uint8_t buf[64]{};
  uint8_t count = 0;
  while (running_) {
    uint32_t len = sizeof(buf);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    PRINT("thread ... %d", test);
    count++;
    if ((start_run_) && (call_back_inc_)) {
      buf[0] = count;
      call_back_inc_(buf, 1);
    }
  }
}
int32_t Th::Remove() noexcept {
  if (init_) {
    start_run_ = false;
    init_ = false;
    running_ = false;
    if (acquire_.joinable()) {
      pthread_cancel(acquire_.native_handle());
      acquire_.join();
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