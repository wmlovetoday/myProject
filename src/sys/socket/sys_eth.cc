#include <cstddef>
#include <cstring>

#include "sys_eth.h"

namespace sys {

/**
struct sockaddr
  {
    char sa_data[14];
  };
struct sockaddr_in
  {
    sa_family_t sin_;
    in_port_t sin_port;
    struct in_addr sin_addr;
  };
struct in_addr
  {
    in_addr_t s_addr;  //typedef uint32_t in_addr_t;
  };
*/

int32_t ConvertNetToLocalAddr(sockaddr_in ip_addr, LocalAddr &addr) {
  constexpr size_t size = sizeof(addr.addr);
  char *p = inet_ntoa(ip_addr.sin_addr);
  if (p != nullptr) {
    memset(addr.addr, 0, size);
    memcpy(reinterpret_cast<void *>(addr.addr), reinterpret_cast<void *>(p), strlen(p));
  } else {
    PRINT_ERRNO("convert addr netaddr : %d failed", ip_addr.sin_addr.s_addr);
    return kSysFailed;
  }
  if (ip_addr.sin_port > 0) {
    addr.host_port = htons(ip_addr.sin_port);
  } else {
    addr.host_port = 0;
  }
  return kSysSuccess;
}

int32_t ConvertSocketToLocalAddr(int32_t fd, sockaddr_in &ip_addr) {
  if (fd > 0) {
    sockaddr addr{};
    socklen_t addrlen = sizeof(struct sockaddr);
    // int getsockname(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
    int32_t ret = getsockname(fd, &addr, &addrlen);
    if (ret == 0) {
      auto *tmp = reinterpret_cast<struct sockaddr_in *>(&addr);
      ip_addr.sin_family = tmp->sin_family;
      ip_addr.sin_addr.s_addr = tmp->sin_addr.s_addr;
      ip_addr.sin_port = tmp->sin_port;
      return kSysSuccess;
    } else {
      PRINT_ERRNO("convert form fd %d to net addr failed", fd);
      return kSysFailed;
    }
  } else {
    PRINT_ERRNO("arguments fd %d illegal", fd);
    return kSysInvalidArgs;
  }
  return kSysSuccess;
}

int32_t ConvertLocalToNetAddr(const LocalAddr &addr, sockaddr_in &ip_addr) {
  if (strlen(addr.addr) == 0) {
    ip_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  } else {
    uint32_t ipaddr_t = 0;
    int32_t ret = inet_aton(addr.addr, reinterpret_cast<struct in_addr *>(&ipaddr_t));
    if (0 == ret) {
      PRINT_ERRNO("convert local addr %s failed", addr.addr);
      return kSysFailed;
    }
    ip_addr.sin_addr.s_addr = ipaddr_t;
  }
  ip_addr.sin_port = htons(addr.host_port);
  return kSysSuccess;
}
}  // namespace sys