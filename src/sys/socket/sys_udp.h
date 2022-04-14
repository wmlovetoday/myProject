#ifndef INC_SYS_SYS_UDP_
#define INC_SYS_SYS_UDP_

#include "sys_eth.h"

namespace sys {

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
}  // namespace sys

#endif