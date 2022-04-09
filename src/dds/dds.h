#ifndef INC_DDS_INSTANCE_H_
#define INC_DDS_INSTANCE_H_

#include <cstdint>
#include <string>
#include <vector>

#ifdef ZMQ_DDS
#include "dds_zmq.h"
#define DDS_INS Ddsz
#endif

namespace dds {

class Dds : public dds::DDS_INS {
 public:
  Dds(CommsType type, const std::string& addr, uint16_t port);

  inline int32_t Read(void* buf, uint32_t len, uint32_t wait_ms, const std::string& topic) override {
    return dds::DDS_INS::Read(buf, len, wait_ms, topic);
  }
  inline int32_t Read(void* buf, uint32_t len, uint32_t wait_ms) { return dds::DDS_INS::Read(buf, len, wait_ms, ""); }

  inline int32_t Write(const void* buf, uint32_t len, const std::string& topic) override {
    return dds::DDS_INS::Write(buf, len, topic);
  }
  inline int32_t Write(const void* buf, uint32_t len) { return dds::DDS_INS::Write(buf, len, ""); }

  ~Dds() = default;
};

}  // namespace dds

#endif