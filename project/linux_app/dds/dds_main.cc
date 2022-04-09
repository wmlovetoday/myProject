

#include <getopt.h>
#include <pthread.h>
#include <stdint.h>

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "base_context.h"
#include "common_log.h"
#include "dds.h"
#include "log_printf.h"

#define STR_FLAG "main"
#define TOPIC "hello"

int32_t main(int32_t argc, char** argv) {
  int32_t ret = 0;
  if (argc < 3) {
    PTS_INFO("must input: ipaddr port index");
    printf("/////// addr: server/client used same ipaddr\n");
    printf("/////// port: server/client used same port\n");
    printf("/////// index: 1(req); 2(rep); 3(pub); 4(sub)\n");
    printf("+++ req +++ ./dds 192.168.1.21 9800 1 \n");
    return -1;
  }
  std::string port_tmp{argv[2]};
  int32_t port = stoi(port_tmp, nullptr, 0);

  std::string index_tmp{argv[3]};
  int32_t index = stoi(index_tmp, nullptr, 0);

  PTS_INFO("{} ipaddr:{} index:{} port:{}", STR_FLAG, argv[1], index, port);

  if (index == 1) {
    dds::Dds com{dds::kCommsReq, std::string{argv[1]},
                 static_cast<uint16_t>(port)};
    std::vector<uint8_t> info{1, 2, 3, 4};
    std::vector<uint8_t> r_info(100, 0);
    while (true) {
      info[0] = info[0] + 1;
      ret = com.Write(info.data(), info.size(), "");
      PTS_INFO("req send {}", ret);
      std::this_thread::sleep_for(std::chrono::seconds(1));
      ret = com.Read(r_info.data(), 10, 1000);
      PTS_INFO("req read {}", ret);
      if (ret > 0) {
        COM_BUF(r_info.data(), ret);
      }
    }
  }
  if (index == 2) {
    PTS_INFO("rep test");
    dds::Dds com{dds::kCommsRep, std::string{argv[1]},
                 static_cast<uint16_t>(port)};
    std::vector<uint8_t> info{5, 6, 7, 8, 9};
    std::vector<uint8_t> r_info(64, 0);

    while (true) {
      ret = com.Read(r_info.data(), r_info.size(), 10000);
      PTS_INFO("rep read {} : ", ret);
      if (ret > 0) {
        COM_BUF(r_info.data(), ret);
      }
      if (ret) {
        ret = com.Write(info.data(), info.size());
        PTS_INFO("rep send {}", ret);
      }
    }
  }
  if (index == 3) {
    PTS_INFO("pub test");
    dds::Dds com{dds::kCommsPub, std::string{argv[1]},
                 static_cast<uint16_t>(port)};
    std::string info{"world"};
    while (true) {
      ret = com.Write(info.data(), info.size(), TOPIC);
      PTS_INFO("pub send {}", ret);
      sleep(1);
    }
  }
  if (index == 4) {
    PTS_INFO("sub test");
    dds::Dds com{dds::kCommsSub, std::string{argv[1]},
                 static_cast<uint16_t>(port)};
    std::string info(100, '\0');
    while (true) {
      ret = com.Read(reinterpret_cast<void*>(const_cast<char*>(info.data())),
                     info.size(), 10000, TOPIC);
      PTS_INFO("sub read {}, {}", ret, info.data());
    }
  }
}
