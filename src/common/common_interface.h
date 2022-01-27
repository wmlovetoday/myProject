#ifndef INC_PT_SEN_COM_INTERFACE_
#define INC_PT_SEN_COM_INTERFACE_
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
#include <set>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace pts {
namespace common {
struct InterfaceInfo {
  std::string interface;
  std::string ipaddr;
  std::string broadcast_addr;
  std::string netmask;
  uint32_t mtu;
};
extern int64_t GetTimeMs();
extern int64_t GetTimeUs();
extern int64_t GetTimeNs();
extern int32_t GetTimeLogo(char (*time_log)[32]);
extern int32_t SysSleep(uint64_t sec);
extern int32_t SysSleepMs(uint64_t ms);
extern int32_t SysSleepNs(uint64_t ns);
extern bool Select(uint8_t input, uint32_t tries);
extern std::set<int32_t> GetIntInput();
extern std::set<int32_t> GetIntInput(std::string &line);

extern int32_t WriteCmd(const char *cmd);
extern int32_t ReadCmd(const char *cmd, std::string &ack, uint32_t try_count = 5);
extern int32_t SdkVersion(std::string &version);

extern int32_t GetInterfaceInfo(std::vector<struct InterfaceInfo> &info);
extern int32_t GetIpAddrByInterfaceName(const std::string interface, std::string &ipaddr);
extern int32_t GetInterfaceNameByIpAddr(const std::string ipaddr, std::string &interface);
// support nest dir
extern int32_t CreateDir(const char *dir, mode_t = 0777);
extern int32_t IsDir(char *dir);

}  // namespace common
}  // namespace pts
#endif