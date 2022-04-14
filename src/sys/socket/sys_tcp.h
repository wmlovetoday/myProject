#ifndef INC_SYS_SYS_TCP_
#define INC_SYS_SYS_TCP_

#include "sys_eth.h"

namespace sys {
class SysTcp {
 public:
  int32_t Open();
  void Close();

  // must call before bind
  int32_t EnableReUseAddr();
  int32_t SetSendBuf(uint32_t size = sys::kSocketSendBufSize);
  int32_t SetRecvBuf(uint32_t size = sys::kSocketRecvBufSize);

  using JudgeReqFunc = std::function<int32_t(const void *src, const void *dst, size_t len)>;
  int32_t Bind(const sockaddr_in &bind_addr);
  int32_t Connect(const sockaddr_in &target_addr);
  int32_t Listen(int32_t max_connect_count = 10);
  int32_t Accept(int32_t &new_fd, sockaddr_in &addr, char *key, JudgeReqFunc fun = memcmp);
  int32_t Accept(int32_t &new_fd, sockaddr_in &addr);

  int32_t SetSendWaitTime(uint32_t ms);
  int32_t SetRecvWaitTime(uint32_t ms);
  int32_t Send(const char *buf, uint32_t len);
  int32_t Send(int32_t tcp_fd, const char *buf, uint32_t len);
  int32_t Recv(int32_t tcp_fd, int8_t *buf, uint32_t len);
  int32_t Recv(int8_t *buf, uint32_t len);
  ~SysTcp();

 private:
  int32_t fd_{-1};
  socklen_t old_send_buf_size_{};
  socklen_t old_recv_buf_size_{};
  socklen_t new_send_buf_size_{};
  socklen_t new_recv_buf_size_{};

  sockaddr_in bind_addr_{};    // bind local addr
  sockaddr_in target_addr_{};  // send to addr
};
}  // namespace sys

#endif