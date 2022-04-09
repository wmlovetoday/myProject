#ifndef INC_DDS_COMMUNICATE_ZMQ_H_
#define INC_DDS_COMMUNICATE_ZMQ_H_

#include <memory>

#include "cc_zmq.h"
#include "dds_data_types.h"
#include "dds_ins.h"

namespace dds {
class Ddsz : public dds::Ddsi, protected zmq::Mq {
 public:
  Ddsz(CommsType type, const std::string& addr, uint16_t port);

  inline int32_t Read(void* buf, uint32_t len, uint32_t wait_ms, const std::string& topic) override {
    return zmq::Mq::Read(buf, len, wait_ms, topic);
  }
  inline int32_t Read(void* buf, uint32_t len, uint32_t wait_ms) { return zmq::Mq::Read(buf, len, wait_ms, ""); }

  inline int32_t Write(const void* buf, uint32_t len, const std::string& topic) override {
    return zmq::Mq::Write(buf, len, topic);
  }
  inline int32_t Write(const void* buf, uint32_t len) { return zmq::Mq::Write(buf, len, ""); }

  virtual ~Ddsz() = default;

 protected:
 private:
};
}  // namespace dds
#endif