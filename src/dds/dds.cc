#include "dds.h"

#define STR_FLAG "dds"

namespace dds {
Dds::Dds(CommsType type, const std::string& addr, uint16_t port) : dds::Ddsz(type, addr, port) {
  PTS_INFO("{} creat dds success", STR_FLAG);
}
}  // namespace dds