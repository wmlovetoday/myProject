#ifndef INC_PT_SEN_SYS_SOCKET_
#define INC_PT_SEN_SYS_SOCKET_

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>  //socket
#include <unistd.h>     // fork

#include <cassert>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <functional>
#include <stdexcept>  // std::out_of_range
#include <thread>
#include <utility>
#include <vector>

#include "log_printf.h"
#include "sys_res.h"

namespace sys {

constexpr socklen_t kSocketSendBufSize{1024 * 10};
constexpr socklen_t kSocketRecvBufSize{1024 * 10};

struct LocalAddr {
  char addr[16];
  uint16_t host_port;
};
extern int32_t ConvertNetToLocalAddr(sockaddr_in ip_addr, LocalAddr &addr);
extern int32_t ConvertSocketToLocalAddr(int32_t fd, sockaddr_in &ip_addr);
extern int32_t ConvertLocalToNetAddr(const LocalAddr &addr, sockaddr_in &ip_addr);

class SysUdp {
 public:
  int32_t Open();
  void Close();

  // must call before bind
  int32_t EnableBroadcast();
  int32_t EnableReUseAddr();
  int32_t SetMulticastTtl(uint8_t ttl);
  int32_t SetSendBuf(uint32_t size = sys::kSocketSendBufSize);
  int32_t SetRecvBuf(uint32_t size = sys::kSocketRecvBufSize);
  int32_t SetSendWaitTime(uint32_t ms);
  int32_t SetRecvWaitTime(uint32_t ms);

  int32_t Bind(const sockaddr_in &bind_addr);
  inline void SetTargetAddr(const sockaddr_in &tar_addr) { memcpy(&target_addr_, &tar_addr, sizeof(sockaddr_in)); }
  int32_t SetMulticastAddr(const char *loc_addr, uint16_t loc_port, const char *tar_addr, uint16_t tar_port);
  int32_t Send(const char *buf, uint32_t len);
  int32_t Send(const char *buf, uint32_t len, const struct sockaddr_in *tar_addr);
  int32_t Recv(char *buf, uint32_t len, sockaddr *addr);

  inline void GetTargetAddr(sockaddr_in *addr) { memcpy(addr, &target_addr_, sizeof(struct sockaddr_in)); }
  inline void GetSrcAddr(sockaddr_in *addr) { memcpy(addr, &bind_addr_, sizeof(struct sockaddr_in)); }
  inline int32_t GetSocketFd() { return fd_; }
  ~SysUdp();

 private:
  int32_t fd_{-1};
  socklen_t old_send_buf_size_{};
  socklen_t old_recv_buf_size_{};
  socklen_t new_send_buf_size_{};
  socklen_t new_recv_buf_size_{};

  // struct sockaddr_in recv_addr_tmp;
  sockaddr_in bind_addr_{};    // bind local addr
  sockaddr_in target_addr_{};  // send to addr
  ip_mreq multiaddr_{};        // IPv4 multicast group
};

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