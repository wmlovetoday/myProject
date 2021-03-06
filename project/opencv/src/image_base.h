#ifndef SRC_IMAGE_BASE_H
#define SRC_IMAGE_BASE_H

#include <memory>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

#include "image_data_defs.h"
#include "log_printf.h"

namespace image {

class ImaBase {
 public:
  virtual void Display(uint32_t width, uint32_t height, const void* memory, const std::string name) = 0;
  inline void SetConvertType(ConvertType type) { type_ = type; }
  inline void SetSavePicType(SavePicType type) {
    if (type == SavePicType::kRaw) {
      save_type_ = SavePicType::kRaw;
      save_pic_type_ = std::string(".raw");
    } else if (type == SavePicType::kBmp) {
      save_type_ = SavePicType::kBmp;
      save_pic_type_ = std::string(".bmp");
    } else if (type == SavePicType::kJpeg) {
      save_type_ = SavePicType::kJpeg;
      save_pic_type_ = std::string(".jpeg");
    } else if (type == SavePicType::kRgb) {
      save_type_ = SavePicType::kJpeg;
      save_pic_type_ = std::string(".rgb");
    }
  }
  inline void SetWaitMs(uint32_t wait_ms) { wait_ms_ = wait_ms; }
  inline void SetVideoFps(uint32_t fps) { video_fps = fps; }
  inline std::string GetWinName() { return win_name_; }
  inline void SetWinName(const std::string& name) { win_name_ = name; }
  inline cv::Mat& GetImage() { return dst_img_; }
  inline uint32_t GetWidth() { return width_; }
  inline uint32_t GetHeight() { return height_; }
  virtual ~ImaBase() {}

 protected:
  bool init_{false};
  std::string win_name_{"win"};
  SavePicType save_type_{SavePicType::kOther};
  std::string save_pic_type_{".bmp"};
  cv::Mat dst_img_{};
  uint32_t wait_ms_{5};
  uint32_t width_;
  uint32_t height_;
  uint16_t video_fps{30};
  uint32_t size_;
  ConvertType type_;
};
}  // namespace image
#endif
