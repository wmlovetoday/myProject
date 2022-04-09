#ifndef SRC_IMAGE_DATA_DEFS_H
#define SRC_IMAGE_DATA_DEFS_H

#include <memory>
#include <string>

namespace image {

constexpr int32_t kImaSuccess = 0;
constexpr int32_t kImaFailed = -1;
constexpr int32_t kImaTimeout = -2;
constexpr int32_t kImaNotSupport = -3;
constexpr int32_t kImaArgsIllegal = -4;

enum class ConvertType {
  kBayer2BG = 1,  // COLOR_BayerBG2BGR = 46
  kBayer2GB,      // COLOR_BayerGB2BGR = 47
  kBayer2RG,      // COLOR_BayerRG2BGR = 48
  kBayer2GR,      // COLOR_BayerGR2BGR = 49

  kYUV2BG,  // CV_YUV2BGR_YUYV
  kYUV2RG,  // CV_YUV2RGB_UYVY

  kYV122RG,  // CV_YUV2RGB_YV12
  kYV122BG   // CV_YUV2BGR_YV12
};

enum class SavePicType {
  kRaw = 1,  // COLOR_BayerBG2BGR = 46
  kBmp,      // COLOR_BayerGB2BGR = 47
  kJpeg,     // COLOR_BayerRG2BGR = 48
  kOther
};

enum SavePicMode { kDefault = 0, kSingle = 1, kContinue };
enum EnableMode { kOFF = 0, kON };

const std::string save_pic_context{"save picture : 1 single    \n               2 continue "};
const std::string save_video_context{"save video   : 1 on        "};
const std::string save_pic_type_context{"save video   : 1 raw  \n               2 bmp, 3 jpeg "};

struct SaveImageTrackbar {
  int32_t trackbar_value;
  int32_t trackbar_value_max;
  std::string pic_dir;
  uint32_t pic_num;
};
struct SavePicTypeTrackbar {
  int32_t trackbar_value;
  int32_t trackbar_value_max;
  std::string pic_dir;
  uint32_t pic_num;
};
struct SaveVideoTrackbar {
  int32_t trackbar_value;
  int32_t trackbar_value_max;
  std::string pic_dir;
};

}  // namespace image
#endif