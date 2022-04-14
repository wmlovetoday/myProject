#include "sys_udp.h"

namespace sys {
int32_t SysUdp::Open() {
  if (fd_ <= 0) {
    fd_ = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd_ < 0) {
      PRINT_ERRNO("creat socket failed");
      return kSysFailed;
    }
  }
  return kSysSuccess;
}

void SysUdp::Close() {
  if (fd_ > 0) close(fd_);
  fd_ = -1;
}

int32_t SysUdp::EnableBroadcast() {
  int32_t ret = 0;
  const int32_t on = 1;
  assert(fd_ > 0);
  if (fd_ <= 0) {
    PRINT("socket not init");
    return kSysFailed;
  }
  ret = setsockopt(fd_, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on));
  if (0 != ret) {
    PRINT_ERRNO("set broadcast attribute failed");
    return kSysFailed;
  }
  return kSysSuccess;
}

int32_t SysUdp::EnableReUseAddr() {
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
int32_t SysUdp::SetMulticastTtl(uint8_t ttl) {
  uint8_t multicastTTL = ttl;  //跨路由数
  assert(fd_ > 0);
  if (fd_ <= 0) {
    PRINT("socket not init");
    return kSysFailed;
  }
  int32_t ret =
      setsockopt(fd_, IPPROTO_IP, IP_MULTICAST_TTL, reinterpret_cast<void *>(&multicastTTL), sizeof(multicastTTL));
  if (ret < 0) {
    PRINT_ERRNO("set multicast option failed");
    return kSysFailed;
  }
  return kSysSuccess;
}
int32_t SysUdp::SetSendBuf(uint32_t size) {
  socklen_t optlen;
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
    PRINT_ERRNO("set udp send buffer size failed");
    return kSysFailed;
  }
  ret = getsockopt(fd_, SOL_SOCKET, SO_SNDBUF, &new_send_buf_size_, &optlen);
  if (ret == 0) {
    PRINT("send buf old %d, now %d", old_send_buf_size_, new_send_buf_size_);
  }
  return kSysSuccess;
}
int32_t SysUdp::SetRecvBuf(uint32_t size) {
  socklen_t optlen;
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
    PRINT_ERRNO("set udp recv buffer size failed");
    return kSysFailed;
  }
  ret = getsockopt(fd_, SOL_SOCKET, SO_RCVBUF, &new_recv_buf_size_, &optlen);
  if (ret == 0) {
    PRINT("recv buf old %d, now %d", old_recv_buf_size_, new_recv_buf_size_);
  }
  return kSysSuccess;
}

int32_t SysUdp::SetSendWaitTime(uint32_t ms) {
  timeval wait{};
  wait.tv_sec = ms / 1000;
  wait.tv_usec += (ms % 1000) * 1000;
  int32_t ret = setsockopt(fd_, SOL_SOCKET, SO_SNDTIMEO, &wait, sizeof(wait));
  if (ret < 0) {
    PRINT_ERRNO("set udp send wait time failed");
    return kSysFailed;
  }
  PRINT("set udp send wait time %d ms", ms);
  return kSysSuccess;
}

int32_t SysUdp::SetRecvWaitTime(uint32_t ms) {
  timeval wait{};
  wait.tv_sec = ms / 1000;
  wait.tv_usec += (ms % 1000) * 1000;

  if (wait.tv_usec >= 1000000) {
    wait.tv_sec += wait.tv_usec / 1000000;
    wait.tv_usec %= 1000000;
  }

  int32_t ret = setsockopt(fd_, SOL_SOCKET, SO_RCVTIMEO, &wait, sizeof(wait));
  if (ret < 0) {
    PRINT_ERRNO("set udp recv wait time failed");
    return kSysFailed;
  }
  PRINT("set udp recv wait time %d ms", ms);
  return kSysSuccess;
}

int32_t SysUdp::Bind(const sockaddr_in &bind_addr) {
  assert(fd_ > 0);
  int32_t ret = bind(fd_, reinterpret_cast<const struct sockaddr *>(&bind_addr), sizeof(struct sockaddr_in));
  if (ret != 0) {
    return kSysFailed;
  }
  memcpy(&bind_addr_, &bind_addr, sizeof(struct sockaddr_in));
  return kSysSuccess;
}

int32_t SysUdp::SetMulticastAddr(const char *loc_addr, uint16_t loc_port, const char *tar_addr, uint16_t tar_port) {
  int32_t ret = 0;
  uint32_t ipaddr_t = 0;
  uint32_t multi_ipaddr = 0;
  assert((nullptr != loc_addr) || (nullptr != tar_addr));
  // must set local ipaddr,otherwise can not recv multi packet
  bind_addr_.sin_family = AF_INET;
  if (strncmp(loc_addr, "0.0.0.0", 7) == 0) {
    PRINT("can not set local ipaddr to %s", loc_addr);
    return kSysFailed;
  }
  ret = inet_aton(loc_addr, reinterpret_cast<struct in_addr *>(&ipaddr_t));
  if (0 == ret) {
    PRINT_ERRNO("convert addr %s to %d failed", loc_addr, ipaddr_t);
    return kSysFailed;
  }
  bind_addr_.sin_addr.s_addr = ipaddr_t;
  bind_addr_.sin_port = loc_port;

  // if add == nullptr,will core dump
  ret = inet_aton(tar_addr, reinterpret_cast<in_addr *>(&multi_ipaddr));
  if (0 == ret) {
    PRINT("convert broadcast_addr %s to %d failed", tar_addr, multi_ipaddr);
    return kSysFailed;
  }
  memset(&multiaddr_, 0, sizeof(multiaddr_));
  multiaddr_.imr_multiaddr.s_addr = multi_ipaddr;
  multiaddr_.imr_interface.s_addr = bind_addr_.sin_addr.s_addr;
  ret = setsockopt(fd_, IPPROTO_IP, IP_ADD_MEMBERSHIP, &multiaddr_, sizeof(multiaddr_));
  if (ret < 0) {
    PRINT_ERRNO("set multaddr %s failed", tar_addr);
    return kSysFailed;
  }
  // copy to target addr,so after SetMulticastAddr can not call SetTargetAddr
  memset(&target_addr_, 0, sizeof(target_addr_));
  target_addr_.sin_family = AF_INET;
  target_addr_.sin_addr.s_addr = multi_ipaddr;
  target_addr_.sin_port = htons(tar_port);
  PRINT("set multaddr and target addr %s:%d success", tar_addr, tar_port);
  return kSysSuccess;
}
int32_t SysUdp::Send(const char *buf, uint32_t len) {
  assert(fd_ > 0);
  assert(nullptr != buf);
  int32_t ret = sendto(fd_, buf, len, 0, reinterpret_cast<sockaddr *>(&target_addr_), sizeof(target_addr_));
  if (ret < 0) {
    PRINT_ERRNO("udp sendto error");
    return kSysFailed;
  }
  return ret;
}
int32_t SysUdp::Send(const char *buf, uint32_t len, const struct sockaddr_in *addr) {
  assert(fd_ > 0);
  assert(nullptr != buf);
  int32_t ret = sendto(fd_, buf, len, 0, reinterpret_cast<sockaddr *>(const_cast<sockaddr_in *>(addr)), sizeof(*addr));
  if (ret < 0) {
    // PRINT_ERRNO("udp sendto error");
    return kSysFailed;
  }
  return ret;
}

int32_t SysUdp::Recv(char *buf, uint32_t len, sockaddr *addr) {
  assert(fd_ > 0);
  assert(nullptr != buf);
  socklen_t addr_len = sizeof(struct sockaddr_in);
  int32_t ret = recvfrom(fd_, buf, len, 0, addr, &addr_len);
  if (ret < 0) {
    // PRINT_ERRNO("udp recvfrom error fd %d", fd_);
    return kSysFailed;
  }
  return ret;
}

SysUdp::~SysUdp() {
  if (fd_ > 0) {
    Close();
    fd_ = -1;
  }
}
}  // namespace sys