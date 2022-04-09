#include "sensor_sys.h"

namespace sensor {

class SensorSys {
 public:
  int32_t SensorSys::GetSize() { return size_; }

  SensorList &GetSensors(bool update_sensors = true);

  void RemoveBySerial(const std::string &serial);

  // SensorPtr GetBySerial(const std::string &serial);
  void Clear();

 private:
  int32_t size_{-1};
};
}  // namespace sensor