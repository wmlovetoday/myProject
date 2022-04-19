

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
#include "sys_udp.h"

#define STR_FLAG "main"

static void UdpTest(sockaddr_in &bind_addr, sockaddr_in &tar_addr) {
  base_con::PrintFlag("udp test");
  sys::SysUdp udp{};
  udp.Open();
  udp.Bind(bind_addr);
  udp.SetTargetAddr(tar_addr);
  char send_time = 10;
  char send_buf[5] = {0};
  char recv_buf[1024] = {0};

  while (send_time) {
    send_buf[0]++;
    udp.Send(send_buf, 1);
    udp.Recv(
        reinterpret_cast<char *>(recv_buf), static_cast<uint32_t>(sizeof(recv_buf)),
        reinterpret_cast<sockaddr *>(&tar_addr));
    PTS_INFO("recv {}", recv_buf);
    // std::this_thread::sleep_for(std::chrono::seconds(1));
    send_time--;
  }
}

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

  sockaddr_in bind_addr{};
  ret = ConvertLocalToNetAddr(tmp_addr, bind_addr);
  if (ret != 0) {
    PRINT_ERRNO("convert bind addr failed");
    return -1;
  }
  bind_addr.sin_family = AF_INET;
  PRINT("bind addr = %s : %d", ip, port);

  sockaddr_in tar_addr{};
  ret = ConvertLocalToNetAddr(target_addr, tar_addr);
  if (ret != 0) {
    PRINT_ERRNO("convert target addr failed");
    return -1;
  }
  tar_addr.sin_family = AF_INET;
  PRINT("target addr = %s : %d", tar_ip, tar_port);

  UdpTest(bind_addr, tar_addr);

  try {
  } catch (std::exception &err) {
    std::cout << "exception:" << err.what() << std::endl;
  } catch (...) {
    std::cout << "exception but do nothing" << std::endl;  // do nothing
    throw;                                                 //重新抛出
  }
  return 0;
}