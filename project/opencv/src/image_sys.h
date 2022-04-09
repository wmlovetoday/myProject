#ifndef SRC_IMAGE_SYS_H
#define SRC_IMAGE_SYS_H

#include <memory>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

#include "com_interface.h"
#include "log_printf.h"

#include "image.h"
#include "image_base.h"
#include "image_data_defs.h"

namespace image {

template <typename T>
class ImagePtr : public T {
 public:
  std::unique_ptr<ImagePtr<T>> GetPtr();

  ~ImagePtr() = default;
};

using PtrByte8 = std::unique_ptr<ImagePtr<ImageByte8>>;
using PtrByte24 = std::unique_ptr<ImagePtr<ImageByte24>>;

class ImaSys {
 public:
  static PtrByte8 GetImageByte8Ptr();
  static PtrByte24 GetImageByte24Ptr();
  static int32_t SaveRawImage(const std::string& save_name, const void* data, uint32_t size);
  static int32_t SaveBmpImage(const std::string& save_name, const cv::Mat& img);
  ~ImaSys();

 private:
};
}  // namespace image
#endif
