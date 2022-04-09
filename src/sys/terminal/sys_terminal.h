#ifndef INC_SYS_TERMINAL_
#define INC_SYS_TERMINAL_

#include <fcntl.h>   /* File control definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <unistd.h>  /* UNIX standard function definitions */

#include <cstdint>
#include <cstring>
#include <string>

#include "com_interface.h"

namespace sys {

class Terminal {
  static constexpr uint32_t kMaxRecvLen = 512;

 public:
  Terminal() { memset(&ter_attrs, 0, sizeof(ter_attrs)); }
  Terminal(const Terminal &) = delete;
  Terminal(Terminal &&) = delete;

  int32_t Open(const std::string &serial);
  int32_t Send(const uint8_t *buffer, uint32_t size);
  int32_t Recv(uint8_t *buffer, uint32_t size);
  int32_t RecvByTerminator(char terminator, uint8_t *buffer, uint32_t size);
  int32_t SelfTest();
  ~Terminal() {
    if (fd > 0) {
      close(fd);
    }
  }

 private:
  int fd{-1};
  std::string ter_name{};
  termios ter_attrs{};
  char recv_buf[kMaxRecvLen]{};
};
};  // namespace sys

#endif
