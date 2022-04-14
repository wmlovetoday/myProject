#ifndef INC_PT_SEN_SYS_ETH_
#define INC_PT_SEN_SYS_ETH_

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

#include "log_printf.h"
#include "sys_defs.h"

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

}  // namespace sys

#endif