

#include <getopt.h>
#include <pthread.h>
#include <stdint.h>

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <set>
#include <string>
#include <thread>
#include <vector>

#include "base_context.h"
#include "base_value.h"
#include "com_interface.h"
#include "common_log.h"
#include "log_printf.h"
#include "sys_th.h"
#include "sys_udp.h"

#define STR_FLAG "main"
using namespace sys;
struct SocArg {
  sockaddr_in bind_addr;
  sockaddr_in tar_addr;
};

ThRunFunc rf = [](const bool &run, const bool &start, void *args) -> int32_t {
  base_con::PrintFlag("udp test");
  SocArg *addr = static_cast<SocArg *>(args);
  sys::SysUdp udp{};
  udp.Open();
  udp.Bind(addr->bind_addr);
  udp.SetTargetAddr(addr->tar_addr);
  char send_buf[5] = {0};
  char recv_buf[1024] = {0};

  while (run) {
    send_buf[0]++;
    udp.Send(send_buf, 1);
    if (start) {
      udp.Recv(
          reinterpret_cast<char *>(recv_buf), static_cast<uint32_t>(sizeof(recv_buf)),
          reinterpret_cast<sockaddr *>(&(addr->tar_addr)));
      PTS_INFO("recv {}", recv_buf);
    } else {
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
  }
  PRINT("exit");
  return 0;
};

int main(int argc, char *argv[]) {
  // base_con::PrintFlag("exception test");
  int32_t ret = 0;
  char ip[] = "192.168.1.21";
  uint16_t port = 8890;

  char tar_ip[] = "192.168.1.114";
  uint16_t tar_port = 8880;

  sys::LocalAddr tmp_addr{};
  tmp_addr.addr = std::string(ip);
  tmp_addr.host_port = port;

  sys::LocalAddr target_addr{};
  target_addr.addr = std::string(tar_ip);
  target_addr.host_port = tar_port;

  SocArg arg{};

  ret = ConvertLocalToNetAddr(tmp_addr, arg.bind_addr);
  if (ret != 0) {
    PRINT_ERRNO("convert bind addr failed");
    return -1;
  }
  arg.bind_addr.sin_family = AF_INET;
  PRINT("bind addr = %s : %d", ip, port);

  ret = ConvertLocalToNetAddr(target_addr, arg.tar_addr);
  if (ret != 0) {
    PRINT_ERRNO("convert target addr failed");
    return -1;
  }
  arg.tar_addr.sin_family = AF_INET;
  PRINT("target addr = %s : %d", tar_ip, tar_port);

  try {
    Th th{};
    // th.Regist(rf, static_cast<void *>(&arg));
    // th.Regist([&](const bool &run, const bool &start, void *args) -> int32_t { return 0; }, static_cast<void
    // *>(&arg));
    th.Regist([&](const bool &run, const bool &start, void *args) -> int32_t { return 0; }, nullptr);
    th.StartCallback();
    uint32_t count = 0;
    while (count < 10) {
      std::this_thread::sleep_for(std::chrono::seconds(1));
      count++;
    }
    th.StopCallback();
    sleep(3);
    th.Remove();
  } catch (std::exception &err) {
    std::cout << "exception:" << err.what() << std::endl;
  } catch (...) {
    std::cout << "exception but do nothing" << std::endl;  // do nothing
    throw;                                                 //重新抛出
  }
  return 0;
}