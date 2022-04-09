#ifndef INS_SENSOR_DEFS_H
#define INS_SENSOR_DEFS_H

// #include <cstdint>
#include <forward_list>
#include <string>

namespace sensor {

struct SensorAttrs {
 public:
  std::string serial;  // number id ? <query>
  uint32_t src_ipaddr;
  uint32_t dst_ipaddr;
  uint16_t src_port;
  uint16_t dst_port;
};

using SensorList = std::forward_list<SensorAttrs>;

}  // namespace sensor

#endif