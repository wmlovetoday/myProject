#ifndef INS_SENSOR_SYS_H
#define INS_SENSOR_SYS_H

#include "sensor_defs.h"

namespace sensor {

class SensorSys {
 public:
  int32_t GetSize();
  SensorList &GetSensors(bool update_sensors = true);
  void RemoveBySerial(const std::string &serial);
  /**
   * @brief Get sensor By Serial
   *
   * @param serial
   * @return SensorPtr
   */
  // SensorPtr GetBySerial(const std::string &serial);
  void Clear();

 private:
  int32_t size_{-1};
};
}  // namespace sensor

#endif