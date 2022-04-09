

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
#include "sys_socket.h"
#include "sys_terminal.h"

#define STR_FLAG "main"

static void TcpTest(sockaddr_in &bind_addr, sockaddr_in &tar_addr) {
  base_con::PrintFlag("tcp test");
  int32_t new_fd;
  sys::SysTcp tcp{};
  tcp.Open();
  tcp.Bind(bind_addr);
  tcp.Listen();
  tcp.Accept(new_fd, tar_addr);
  char send_time = 10;
  char send_buf[5] = {0};
  char recv_buf[1024] = {0};

  while (send_time) {
    send_buf[0]++;
    tcp.Send(new_fd, send_buf, 1);
    tcp.Recv(new_fd, reinterpret_cast<int8_t *>(recv_buf), static_cast<uint32_t>(sizeof(recv_buf)));
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
  strcpy(tmp_addr.addr, ip);
  tmp_addr.host_port = port;

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

  TcpTest(bind_addr, tar_addr);

  try {
  } catch (std::exception &err) {
    std::cout << "exception:" << err.what() << std::endl;
  } catch (...) {
    std::cout << "exception but do nothing" << std::endl;  // do nothing
    throw;                                                 //重新抛出
  }
  return 0;
}