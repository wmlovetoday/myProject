#ifndef INC_DDS_DATA_TYPES_H_
#define INC_DDS_DATA_TYPES_H_

#include <cstdint>
#include <string>
#include <vector>

namespace dds {
constexpr int32_t kCommuSuccess = 0;
constexpr int32_t kCommuFailed = -1;
constexpr int32_t kCommuNotSupport = -2;
constexpr int32_t kCommuTimeout = -3;
enum CommsType { kCommsPub = 1, kCommsSub, kCommsReq, kCommsRep };
}  // namespace dds

#endif