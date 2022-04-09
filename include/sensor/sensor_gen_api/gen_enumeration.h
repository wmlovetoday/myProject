#ifndef INS_GEN_API_GEN_ENUM_H
#define INS_GEN_API_GEN_ENUM_H

#include "enumeration.h"

namespace sensor {
namespace gen_api {

class Enumeration {
  bool IsAvailable();
  int32_t SetIntValue(int64_t Value);
  int32_t GetIntValue(int64_t& Value);
};

}  // namespace gen_api
}  // namespace sensor

#endif