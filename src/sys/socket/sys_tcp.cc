#include "sys_tcp.h"

namespace sys {
int32_t SysTcp::Open() {
  if (fd_ <= 0) {
    fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (fd_ < 0) {
      PRINT_ERRNO("creat socket failed");
      return kSysFailed;
    }
  }
  return kSysSuccess;
}
void SysTcp::Close() {
  if (fd_ > 0) {
    close(fd_);
    PRINT("close %d", fd_);
  }
  fd_ = -1;
}

int32_t SysTcp::EnableReUseAddr() {
  int32_t ret = 0;
  const int32_t on = 1;
  assert(fd_ > 0);
  if (fd_ <= 0) {
    PRINT("socket not init");
    return kSysFailed;
  }
  ret = setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
  if (0 != ret) {
    PRINT_ERRNO("set reuse addr attribute failed\n");
    return kSysFailed;
  }
  return kSysSuccess;
}
int32_t SysTcp::SetSendBuf(uint32_t size) {
  socklen_t optlen{};
  assert(fd_ > 0);
  if (fd_ <= 0) {
    PRINT("socket not init");
    return kSysFailed;
  }
  int32_t ret = getsockopt(fd_, SOL_SOCKET, SO_SNDBUF, &old_send_buf_size_, &optlen);
  optlen = sizeof(size);
  if (0 == size) {
    new_send_buf_size_ = kSocketSendBufSize;
  } else {
    new_send_buf_size_ = size;
  }
  ret = setsockopt(fd_, SOL_SOCKET, SO_SNDBUF, reinterpret_cast<void *>(&new_send_buf_size_), optlen);
  if (ret < 0) {
    PRINT_ERRNO("set tcp send buffer size failed");
    return kSysFailed;
  }
  ret = getsockopt(fd_, SOL_SOCKET, SO_SNDBUF, &new_send_buf_size_, &optlen);
  if (ret == 0) {
    PRINT("fd %d send buf old %d, now %d", fd_, old_send_buf_size_, new_send_buf_size_);
  }
  return kSysSuccess;
}
int32_t SysTcp::SetRecvBuf(uint32_t size) {
  socklen_t optlen{};
  assert(fd_ > 0);
  if (fd_ <= 0) {
    PRINT("socket not init");
    return kSysFailed;
  }
  int32_t ret = getsockopt(fd_, SOL_SOCKET, SO_RCVBUF, &old_recv_buf_size_, &optlen);
  optlen = sizeof(size);
  if (0 == size) {
    new_recv_buf_size_ = kSocketRecvBufSize;
  } else {
    new_recv_buf_size_ = size;
  }
  ret = setsockopt(fd_, SOL_SOCKET, SO_RCVBUF, reinterpret_cast<void *>(&new_recv_buf_size_), optlen);
  if (ret < 0) {
    PRINT_ERRNO("set tcp recv buffer size failed");
    return kSysFailed;
  }
  ret = getsockopt(fd_, SOL_SOCKET, SO_RCVBUF, &new_recv_buf_size_, &optlen);
  if (ret == 0) {
    PRINT("recv buf old %d, now %d", old_recv_buf_size_, new_recv_buf_size_);
  }
  return kSysSuccess;
}

int32_t SysTcp::Bind(const sockaddr_in &bind_addr) {
  assert(fd_ > 0);
  int32_t ret = bind(fd_, reinterpret_cast<const struct sockaddr *>(&bind_addr), sizeof(struct sockaddr_in));
  if (ret != 0) {
    return kSysFailed;
  }
  memcpy(&bind_addr_, &bind_addr, sizeof(struct sockaddr_in));
  return kSysSuccess;
}

int32_t SysTcp::SetSendWaitTime(uint32_t ms) {
  timeval wait{};
  wait.tv_sec = ms / 1000;
  wait.tv_usec += (ms % 1000) * 1000;
  int32_t ret = setsockopt(fd_, SOL_SOCKET, SO_SNDTIMEO, &wait, sizeof(wait));
  if (ret < 0) {
    PRINT_ERRNO("set tcp send wait time failed");
    return kSysFailed;
  }
  PRINT("set tcp send wait time %d ms", ms);
  return kSysSuccess;
}

int32_t SysTcp::SetRecvWaitTime(uint32_t ms) {
  timeval wait{};
  wait.tv_sec = ms / 1000;
  wait.tv_usec += (ms % 1000) * 1000;

  if (wait.tv_usec >= 1000000) {
    wait.tv_sec += wait.tv_usec / 1000000;
    wait.tv_usec %= 1000000;
  }

  int32_t ret = setsockopt(fd_, SOL_SOCKET, SO_RCVTIMEO, &wait, sizeof(wait));
  if (ret < 0) {
    PRINT_ERRNO("set tcp recv wait time failed");
    return kSysFailed;
  }
  PRINT("set tcp recv wait time %d ms", ms);
  return kSysSuccess;
}
int32_t SysTcp::Connect(const sockaddr_in &target_addr) {
  assert(fd_ > 0);
  int32_t ret = connect(fd_, reinterpret_cast<const struct sockaddr *>(&target_addr), sizeof(struct sockaddr));
  if (ret < 0) {
    return kSysFailed;
  }
  return kSysSuccess;
}
int32_t SysTcp::Listen(int32_t max_connect_count) {
  assert(fd_ > 0);
  int32_t ret = listen(fd_, max_connect_count);
  if (ret < 0) {
    PRINT_ERRNO("listen failed");
    return kSysFailed;
  }
  return kSysSuccess;
}
int32_t SysTcp::Accept(int32_t &new_fd, sockaddr_in &addr, char *key, JudgeReqFunc fun) {
  assert(fd_ > 0);
  assert(key != nullptr);
  socklen_t addr_len = sizeof(struct sockaddr_in);
  int32_t acc_fd = accept(fd_, reinterpret_cast<struct sockaddr *>(&addr), &addr_len);
  if (acc_fd < 0) {
    PRINT_ERRNO("accept failed");
    return kSysFailed;
  }
  new_fd = acc_fd;
  if ((fun != nullptr) && (key != nullptr)) {
    int8_t buf[256]{};
    int32_t len = recvfrom(acc_fd, &buf[0], sizeof(buf), 0, reinterpret_cast<struct sockaddr *>(&addr), &addr_len);
    if (len > 0) {
      int32_t ret = fun(reinterpret_cast<void *>(buf), reinterpret_cast<void *>(key), len);
      if (ret == 0) {
        return kSysSuccess;
      }
      PRINT("recv not comapre");
      return kSysFailed;
    }
    PRINT_ERRNO("recv failed %d", len);
    return kSysFailed;
  }
  return kSysSuccess;
}
int32_t SysTcp::Accept(int32_t &new_fd, sockaddr_in &addr) {
  assert(fd_ > 0);
  socklen_t addr_len = sizeof(struct sockaddr_in);
  int32_t acc_fd = accept(fd_, reinterpret_cast<struct sockaddr *>(&addr), &addr_len);
  if (acc_fd < 0) {
    PRINT_ERRNO("accept failed");
    return kSysFailed;
  }
  new_fd = acc_fd;
  return kSysSuccess;
}
int32_t SysTcp::Send(int32_t tcp_fd, const char *buf, uint32_t len) {
  assert(tcp_fd > 0);
  assert(nullptr != buf);
  assert(len > 0);
  int32_t ret = send(tcp_fd, buf, len, MSG_NOSIGNAL);
  if (ret < 0) {
    PRINT_ERRNO("fd %d tcp send error", fd_);
    return kSysFailed;
  }
  return ret;
}
int32_t SysTcp::Send(const char *buf, uint32_t len) {
  assert(fd_ > 0);
  assert(nullptr != buf);
  assert(len > 0);
  int32_t ret = send(fd_, buf, len, MSG_NOSIGNAL);
  if (ret < 0) {
    PRINT_ERRNO("fd %d tcp send error", fd_);
    return kSysFailed;
  }
  return ret;
}
int32_t SysTcp::Recv(int32_t tcp_fd, int8_t *buf, uint32_t len) {
  assert(tcp_fd > 0);
  assert(nullptr != buf);
  assert(len > 0);
  int32_t ret = recv(tcp_fd, buf, len, 0);
  if (ret < 0) {
    PRINT_ERRNO("fd %d tcp recv error", fd_);
    return kSysFailed;
  }
  return ret;
}

int32_t SysTcp::Recv(int8_t *buf, uint32_t len) {
  assert(fd_ > 0);
  assert(nullptr != buf);
  assert(len > 0);
  int32_t ret = recv(fd_, buf, len, 0);
  if (ret < 0) {
    PRINT_ERRNO("fd %d tcp recv error", fd_);
    return kSysFailed;
  }
  return ret;
}
SysTcp::~SysTcp() {
  if (fd_ > 0) {
    PRINT("close %d", fd_);
    Close();
    fd_ = -1;
  }
}
}  // namespace sys