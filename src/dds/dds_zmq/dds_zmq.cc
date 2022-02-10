#include "dds_zmq.h"

#define STR_FLAG "dds:zmq"

namespace dds {
Ddsz::Ddsz(CommsType type, const std::string& addr, uint16_t port) : zmq::Mq(type, addr, port) {
  PTS_INFO("{} creat mq success", STR_FLAG);
}
}  // namespace dds