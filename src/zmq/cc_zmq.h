#ifndef INC_COMMUNICATE_COMPONENT_ZMQ_H_
#define INC_COMMUNICATE_COMPONENT_ZMQ_H_

#include <arpa/inet.h>
#include <ifaddrs.h>
#include <sys/types.h>

#include <cassert>
#include <cstdint>
#include <exception>
#include <stdexcept>
#include <string>
#include <vector>

#include <zmq.h>

#include "common_log.h"
#include "dds_data_types.h"
#include "log_printf.h"

namespace zmq {
constexpr uint32_t LingerPeriodMs = (1000);

class Mq {
 public:
  /**
   * req and sub : connect to remote's ip:port
   * rep and pub : bind local's ip/interface:port
   */
  Mq(dds::CommsType type, const std::string &addr, uint16_t port);
  Mq(const Mq &) = delete;
  Mq(const Mq &&) = delete;
  Mq &operator=(const Mq &) = delete;
  Mq &operator=(const Mq &&) = delete;

  /**
   * return read/write length,if >0
   */
  int32_t Read(void *buf, uint32_t len, uint32_t wait_ms, const std::string &topic = "");
  int32_t Write(const void *buf, uint32_t len, const std::string &topic = "");
  virtual ~Mq() { RemoveMq(); };

 protected:
  bool init_{false};
  void *context_{nullptr};
  void *socket_{nullptr};
  dds::CommsType mq_type_{};
  std::string addr_{};
  std::string topic_{};
  uint16_t port_{};

 private:
  int32_t Init();
  void RemoveMq();
  int32_t Send(const void *buf, size_t len, int flags);
  int32_t Receive(zmq_msg_t &part, uint32_t wait_ms, const std::string &topic);
};
}  // namespace zmq
#endif