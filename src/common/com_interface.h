#ifndef INC_SRC_COM_INTERFACE_
#define INC_SRC_COM_INTERFACE_
#include <arpa/inet.h>
#include <dirent.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>

#include <cassert>
#include <cerrno>
#include <cstdio>
#include <iostream>
#include <mutex>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "log_printf.h"

namespace com {

constexpr int32_t kComSuccess = 0;
constexpr int32_t kComFailed = -1;
constexpr int32_t kComTimeout = -2;
constexpr int32_t kComNotSupport = -3;
constexpr int32_t kComInvalidArgs = -4;

struct InterfaceInfo {
  std::string interface;
  std::string ipaddr;
  std::string broadcast_addr;
  std::string netmask;
  uint32_t mtu;
};
int64_t GetTimeMs();
int64_t GetTimeUs();
int64_t GetTimeNs();
int32_t GetTimeLogo(int8_t (*time_log)[32]);
int32_t SysSleep(uint64_t sec);
int32_t SysSleepMs(uint64_t ms);
int32_t SysSleepNs(uint64_t ns);
bool Select(uint8_t input, uint32_t tries);
std::vector<int64_t> GetIntInput();
std::vector<int64_t> GetIntInput(std::string &line);
int32_t WriteCmd(const char *cmd);
int32_t ReadCmd(const char *cmd, std::string &ack, uint32_t try_count = 5);
int32_t SdkVersion(std::string &version);

int32_t GetInterfaceInfo(std::vector<struct InterfaceInfo> &info);
int32_t GetIpAddrByInterfaceName(const std::string interface, std::string &ipaddr);
int32_t GetInterfaceNameByIpAddr(const std::string ipaddr, std::string &interface);
// support nest dir
int32_t CreateDir(const char *dir, mode_t = 0777);
int32_t IsDir(char *dir);
int32_t SpiltStr(const std::string &src, const std::string &compare, std::vector<std::string> &ret);
int32_t GetPid(std::string &process, std::vector<uint32_t> &pid);
int32_t GetPagesize();
}  // namespace com

#endif