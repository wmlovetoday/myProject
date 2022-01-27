#include "common_interface.h"

#define TIME_OUT_FORMAT_ "%Y-%m-%d_%H-%M-%S"
#define STR_FLAG "common:interface "
#define CONTENT_ __func__, __LINE__
#define PRINT_LINE printf("%s %s %d\n", STR_FLAG, CONTENT_)
#define PRINT_ERRNO(format, ...) \
  printf("%s" format " syserr:%s:%d, %s:%d\n", STR_FLAG, ##__VA_ARGS__, strerror(errno), errno, __func__, __LINE__);
#define PRINT(format, ...)                                                     \
  printf("%s" format " %s:%d\n", STR_FLAG, ##__VA_ARGS__, __func__, __LINE__); \
  fflush(stdout)

namespace pts {
int64_t common::GetTimeMs() {
  int64_t ms = 0;
  struct timeval tv;
  int32_t ret = gettimeofday(&tv, nullptr);
  if (0 == ret) {
    ms = tv.tv_usec;
    ms /= 1000;
    ms += (tv.tv_sec * 1000);
    return ms;
  }
  PRINT_ERRNO("func gettimeofday fail");
  return -1;
}
int64_t common::GetTimeUs() {
  int64_t us;
  struct timeval tv;
  int32_t ret = gettimeofday(&tv, nullptr);
  if (0 == ret) {
    us = tv.tv_usec + (tv.tv_sec * 1000000);
    return us;
  }
  PRINT_ERRNO("func gettimeofday fail");
  return -1;
}
//系统实时时间,随系统实时时间改变而改变,即从UTC1970-1-1
// 0:0:0开始计时,中间时刻如果系统时间被用户该成其他,则对应的时间相应改变
//  struct timespec {
//                time_t   tv_sec;        /* seconds */
//                long     tv_nsec;       /* nanoseconds */
//            };
// typedef long time_t

// CLOCK_REALTIME  墙上时钟
// CLOCK_MONOTONIC 单调递增的时钟,一般会把系统启动的时间点设定为其基准点
// CLOCK_MONOTONIC_RAW 完全基于本地晶振的时钟,不能设定
// CLOCK_PROCESS_CPUTIME_ID 进程时钟
// CLOCK_THREAD_CPUTIME_ID 线程时钟
// int32_t clock_getcpuclockid(pid_t pid, clockid_t *clock_id);
// int32_t pthread_getcpuclockid(pthread_t thread, clockid_t *clock_id);

int64_t common::GetTimeNs() {
  int64_t ns = 0;
  struct timespec tv;
  int32_t ret = clock_gettime(CLOCK_REALTIME, &tv);  // tv_sec
  if (0 == ret) {
    ns = tv.tv_sec * 1000000000;
    ns += tv.tv_nsec;
    return ns;
  }
  PRINT_ERRNO("func clock_gettime fail");
  return -1;
}
int32_t common::GetTimeLogo(char (*time_log)[32]) {
  char s_time[sizeof(*time_log)] = {'\0'};
  size_t len = sizeof(*time_log);
  if (nullptr == time_log) return -1;
  time_t timer = time(nullptr);
  if ((static_cast<time_t>(-1)) != timer) {
    strftime(s_time, sizeof(s_time), TIME_OUT_FORMAT_, localtime(&timer));
    memcpy(reinterpret_cast<void *>(time_log), reinterpret_cast<void *>(s_time), len);
    char *tmp = reinterpret_cast<char *>(time_log);
    *(tmp + len - 1) = '\0';
    return 0;
  }
  PRINT_ERRNO("func time fail");
  return -2;
}
int32_t common::SysSleep(uint64_t sec) {
  int32_t ret = 0;
  struct timespec req, rem;
  req.tv_sec = sec;
  req.tv_nsec = 0;
  while (true) {
    ret = nanosleep(&req, &rem);
    if (ret == 0) {
      break;
    }
    req.tv_sec = rem.tv_sec;
    req.tv_nsec = rem.tv_nsec;
  }
  return (ret);
}
int32_t common::SysSleepMs(uint64_t ms) { return (common::SysSleepNs(ms * 1000000)); }
int32_t common::SysSleepNs(uint64_t ns) {
  int32_t ret = 0;
  struct timespec req, rem;
  req.tv_sec = ns / 1000000000;
  req.tv_nsec = ns % 1000000000;
  while (true) {
    ret = nanosleep(&req, &rem);
    if (ret == 0) {
      break;
    }
    req.tv_sec = rem.tv_sec;
    req.tv_nsec = rem.tv_nsec;
  }
  return (ret);
}

bool common::Select(uint8_t /*input*/, uint32_t /*tries*/) {
  uint32_t tmp = 1;
  while (tmp < 4) {
    std::cout << "Do you want to proceed(y or n)?\n";
    char answer = 0;
    std::cin >> answer;
    if (isalpha(answer)) {
      switch (answer) {
        case 'y':
        case 'Y':
          return true;
        case 'n':
        case 'N':
          return false;
        default:
          std::cout << "sorry,input error.\n";
          ++tmp;
          break;
      }
    }
  }
  return false;
}

std::set<int32_t> common::GetIntInput() {
  std::set<int32_t> ret;
  int32_t begin = -1;
  int32_t end = -1;
  int32_t value = -100;
  while (true) {
    std::string word, line;
    std::cout << "input Decimal(default) or Hexadecimal(0x or 0X)" << std::endl;
    std::getline(std::cin, line);  //遇到\n结束
    if (line.size() > 0) {
      for (size_t i = 0; i <= line.size(); i++) {
        // std::cout<<"line.size= "<<line.size()<<" i= "<<i<<" "<<line[i]<<" "<<(int32_t)(line[i])<<std::endl;
        if (-1 == begin) {
          if (!isspace(line[i])) {
            begin = i;
            // std::cout<<"begin is "<< line[i]<<std::endl;
          }
        } else {
          if (-1 == end) {
            if ((isspace(line[i])) || (0 == static_cast<int32_t>(line[i])))  // space or '\n'
            {
              uint8_t valid = 0;
              end = i;
              for (uint8_t j = begin; j < end; j++) {
                if (!isxdigit(line[begin]))  // not 0~9 a~f A~F
                {
                  if ((line[begin] == 'x') || (line[begin] == 'X')) {
                    if (j == i + 1) {
                      valid = 1;
                    } else {
                      valid = 0;
                    }
                  } else {
                    valid = 0;
                  }
                } else {
                  valid = 1;
                }
              }
              if (valid) {
                word.assign(line.begin() + begin, line.begin() + end);
                value = stoi(word, nullptr, 0);
                ret.insert(value);
              }
              begin = -1;
              end = -1;
            }
          }
        }
      }
      break;
    }
    // else
    //     std::cout << "input length illegal" << std::endl;
  }
  return ret;
}

std::set<int32_t> common::GetIntInput(std::string& line) {
  std::set<int32_t> ret;
  int32_t begin = -1;
  int32_t end = -1;
  int32_t value = -100;
  while (true) {
    std::string word{};
    if (line.size() > 0) {
      for (size_t i = 0; i <= line.size(); i++) {
        // std::cout<<"line.size= "<<line.size()<<" i= "<<i<<" "<<line[i]<<" "<<(int32_t)(line[i])<<std::endl;
        if (-1 == begin) {
          if (!isspace(line[i])) {
            begin = i;
            // std::cout<<"begin is "<< line[i]<<std::endl;
          }
        } else {
          if (-1 == end) {
            if ((isspace(line[i])) || (0 == static_cast<int32_t>(line[i])))  // space or '\n'
            {
              uint8_t valid = 0;
              end = i;
              for (uint8_t j = begin; j < end; j++) {
                if (!isxdigit(line[begin]))  // not 0~9 a~f A~F
                {
                  if ((line[begin] == 'x') || (line[begin] == 'X')) {
                    if (j == i + 1) {
                      valid = 1;
                    } else {
                      valid = 0;
                    }
                  } else {
                    valid = 0;
                  }
                } else {
                  valid = 1;
                }
              }
              if (valid) {
                word.assign(line.begin() + begin, line.begin() + end);
                value = stoi(word, nullptr, 0);
                ret.insert(value);
              }
              begin = -1;
              end = -1;
            }
          }
        }
      }
      break;
    }
    // else
    //     std::cout << "input length illegal" << std::endl;
  }

  return ret;
}

int32_t common::WriteCmd(const char *cmd) {
  if (cmd == nullptr) return -1;
  FILE *pp = popen(cmd, "w");
  if (pp == nullptr) {
    PRINT_ERRNO("popen failed");
    return -1;
  }
  int32_t ret = pclose(pp);
  return ret;
}
int32_t common::ReadCmd(const char *cmd, std::string &ack, uint32_t try_count) {
  uint32_t tmp = try_count;
  char recv[1024];
  if (cmd == nullptr) return -1;
  ack.clear();
  FILE *pp = popen(cmd, "r");
  if (pp == nullptr) {
    PRINT_ERRNO("popen failed");
    return -2;
  }
  while (tmp) {
    char *s = fgets(recv, sizeof(recv), pp);
    if (s == nullptr) {
      PRINT_ERRNO("%d fgets failed", tmp);
      pclose(pp);
      return -1;
    }
    ack += recv;
    break;
    --tmp;
    // }
  }
  pclose(pp);
  if (tmp) {
    return 0;
  }
  return -4;
}
/*
 1 struct ifaddrs
 2 {
 3     struct ifaddrs  *ifa_next;    // Next item in list
 4     char            *ifa_name;    // Name of interface
 5     unsigned int     ifa_flags;   // Flags from SIOCGIFFLAGS
 6     struct sockaddr *ifa_addr;    // Address of interface
 7     struct sockaddr *ifa_netmask; // Netmask of interface
 8     union
 9     {
10         struct sockaddr *ifu_broadaddr; // Broadcast address of interface
11         struct sockaddr *ifu_dstaddr; // Point-to-point destination address
12     } ifa_ifu;
13     #define              ifa_broadaddr ifa_ifu.ifu_broadaddr
14     #define              ifa_dstaddr   ifa_ifu.ifu_dstaddr
15     void            *ifa_data;    	// Address-specific data
16 };
ifa_next 指向链表的下一个成员；
ifa_name 是接口名称，以0结尾的字符串，比如eth0，lo；
ifa_flags是接口的标识位（比如当IFF_BROADCAST或IFF_POINTOPOINT设置到此标识位时，影响联合体变量ifu_broadaddr存储广播地址或ifu_dstaddr记录点对点地址）；
ifa_netmask存储该接口的子网掩码；
结构体变量存储广播地址或点对点地址（见括弧介绍ifa_flags）；
ifa_data存储了该接口协议族的特殊信息，它通常是NULL（一般不关注他）。

函数getifaddrs（int getifaddrs (struct ifaddrs **__ifap)）获取本地网络接口信息，将之存储于链表中，
链表头结点指针存储于__ifap中带回，函数执行成功返回0，失败返回-1，且为errno赋值。
    很显然，函数getifaddrs用于获取本机接口信息，比如最典型的获取本机IP地址。
*/

int32_t common::GetInterfaceInfo(std::vector<struct InterfaceInfo> &info) {
  int32_t ret = 0;
  struct ifaddrs *ifAddrStruct = nullptr;
  void *tmpAddrPtr = nullptr;
  char addressBuffer[INET6_ADDRSTRLEN];
  // uint32_t len = sizeof(*interface);
  info.clear();
  InterfaceInfo info_t{};
  ret = getifaddrs(&ifAddrStruct);
  if (0 == ret) {
    while (ifAddrStruct != nullptr) {
      // PRINT("----------- %s", ifAddrStruct->ifa_name);
      if (ifAddrStruct->ifa_addr->sa_family == AF_INET) {
        // is a valid IP4 Address
        if (strcmp(ifAddrStruct->ifa_name, "lo") != 0) {
          tmpAddrPtr = &(reinterpret_cast<struct sockaddr_in *>(ifAddrStruct->ifa_addr))->sin_addr;
          inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
          // PRINT("%s IP Address %s", ifAddrStruct->ifa_name, addressBuffer);
          info_t.interface.clear();
          info_t.ipaddr.clear();
          info_t.interface = ifAddrStruct->ifa_name;
          info_t.ipaddr = addressBuffer;
          tmpAddrPtr = &(reinterpret_cast<struct sockaddr_in *>(ifAddrStruct->ifa_netmask))->sin_addr;
          inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
          // PRINT("mask %s", addressBuffer);
          info_t.netmask.clear();
          info_t.netmask = addressBuffer;
          tmpAddrPtr = &(reinterpret_cast<struct sockaddr_in *>(ifAddrStruct->ifa_ifu.ifu_broadaddr))->sin_addr;
          inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
          // PRINT("broadcast %s", addressBuffer);
          info_t.broadcast_addr.clear();
          info_t.broadcast_addr = addressBuffer;
          std::string cmd{};
          std::string ack{};
          cmd += "cat /sys/class/net/";
          cmd += info_t.interface;
          cmd += "/mtu";
          common::ReadCmd(cmd.data(), ack);
          info_t.mtu = stoi(ack, nullptr, 0);
          // PRINT("mtu %d", info_t.mtu);
          info.push_back(info_t);
        }
      } /*
       else if (ifAddrStruct->ifa_addr->sa_family == AF_INET6)
       {  // is a valid IP6 Address
           if (strcmp(ifAddrStruct->ifa_name, "lo") != 0)
           {
               tmpAddrPtr = &((struct sockaddr_in6 *)(ifAddrStruct->ifa_addr))->sin6_addr;
               inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);
               PRINT("%s IP Address %s", ifAddrStruct->ifa_name, addressBuffer);
               info_t.interface.clear();
               info_t.ipaddr.clear();
               info_t.interface = ifAddrStruct->ifa_name;
               info_t.ipaddr = addressBuffer;
               tmpAddrPtr = &((struct sockaddr_in6 *)ifAddrStruct->ifa_netmask)->sin6_addr;
               inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
               // PRINT("mask %s", addressBuffer);
               info_t.netmask.clear();
               info_t.netmask = addressBuffer;
               tmpAddrPtr = &((struct sockaddr_in6 *)ifAddrStruct->ifa_ifu.ifu_broadaddr)->sin6_addr;
               inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
               PRINT("broadcast %s", addressBuffer);
               info_t.broadcast_addr.clear();
               info_t.broadcast_addr = addressBuffer;
               std::string cmd{};
               std::string ack{};
               cmd += "cat /sys/class/net/";
               cmd += info_t.interface;
               cmd += "/mtu";
               common::ReadCmd(cmd.data(), ack);
               info_t.mtu = stoi(ack, nullptr, 0);
               info.push_back(info_t);
           }
       }*/
      ifAddrStruct = ifAddrStruct->ifa_next;
    }
  } else {
    PRINT_ERRNO("getifaddrs failed");
    return -1;
  }
  return 0;
}

int32_t common::GetIpAddrByInterfaceName(const std::string interface, std::string &ipaddr) {
  assert(interface != "");
  char addressBuffer[INET6_ADDRSTRLEN];
  void *tmpAddrPtr = nullptr;
  struct ifaddrs *ifAddrStruct = nullptr;
  int32_t ret = getifaddrs(&ifAddrStruct);
  if (0 == ret) {
    while (ifAddrStruct != nullptr) {
      if (strncmp(ifAddrStruct->ifa_name, interface.c_str(), interface.length()) == 0) {
        if (ifAddrStruct->ifa_addr->sa_family == AF_INET) {  // is a valid IP4 Address
          tmpAddrPtr = &(reinterpret_cast<struct sockaddr_in *>(ifAddrStruct->ifa_addr))->sin_addr;
          inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
        } else if (ifAddrStruct->ifa_addr->sa_family == AF_INET6) {  // is a valid IP6 Address
          tmpAddrPtr = &(reinterpret_cast<struct sockaddr_in *>(ifAddrStruct->ifa_addr))->sin_addr;
          inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);
        }
        ipaddr.assign(addressBuffer);
        return 0;
      }
      ifAddrStruct = ifAddrStruct->ifa_next;
    }
  } else {
    PRINT_ERRNO("getifaddrs failed");
    return -1;
  }
  return -1;
}

int32_t common::GetInterfaceNameByIpAddr(const std::string ipaddr, std::string &interface) {
  assert(ipaddr != "");
  char addressBuffer[INET6_ADDRSTRLEN];
  void *tmpAddrPtr = nullptr;
  struct ifaddrs *ifAddrStruct = nullptr;
  int32_t ret = getifaddrs(&ifAddrStruct);
  if (0 == ret) {
    while (ifAddrStruct != nullptr) {
      if (ifAddrStruct->ifa_addr->sa_family == AF_INET) {  // is a valid IP4 Address
        tmpAddrPtr = &(reinterpret_cast<struct sockaddr_in *>(ifAddrStruct->ifa_addr))->sin_addr;
        inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
      } else if (ifAddrStruct->ifa_addr->sa_family == AF_INET6) {  // is a valid IP6 Address
        tmpAddrPtr = &(reinterpret_cast<struct sockaddr_in *>(ifAddrStruct->ifa_addr))->sin_addr;
        inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);
      }

      if (strncmp(addressBuffer, ipaddr.c_str(), ipaddr.length()) == 0) {
        interface.assign(ifAddrStruct->ifa_name);
        return 0;
      }
      ifAddrStruct = ifAddrStruct->ifa_next;
    }
  } else {
    PRINT_ERRNO("getifaddrs failed");
    return -1;
  }
  return -1;
}

int32_t common::CreateDir(const char *dir, mode_t mode) {
  assert(dir != nullptr);
  umask(0);
  char tmp[512]{};
  for (size_t i = 0; i < (strlen(dir) + 1); i++) {
    if ((dir[i] == '/') || (dir[i] == '\0')) {
      uint32_t j = 0;
      for (j = 0; j < i; j++) {
        tmp[j] = dir[j];
      }
      tmp[j] = 0;
      if (IsDir(tmp) != 0) {
        int32_t ret = mkdir(tmp, mode);
        if (ret != 0) {
          PRINT("mkdir %s failed", tmp);
          PRINT_ERRNO();
          return -1;
        }
      }
    }
  }
  return 0;
}
int32_t common::IsDir(char *dir) {
  assert(dir != nullptr);
  struct stat dir_buf;
  if (nullptr == dir) return -1;
  if (stat(dir, &dir_buf) == -1) {
    // PRINT_ERRNO("func stat fail");
    return -1;
  }
  if (S_ISDIR(dir_buf.st_mode)) {
    return 0;
  }

  return -1;
}
}  // namespace pts