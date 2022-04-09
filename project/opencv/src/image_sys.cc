
#include "image_sys.h"
#include "log_printf.h"

namespace image {

template <typename T>
std::unique_ptr<ImagePtr<T>> ImagePtr<T>::GetPtr() {
  ImagePtr<T> tmp;
  std::unique_ptr<ImagePtr<T>> ptr = std::make_unique<ImagePtr<T>>(tmp);
  return ptr;
}

PtrByte8 ImaSys::GetImageByte8Ptr() {
  ImagePtr<ImageByte8> tmp;
  return tmp.GetPtr();
}

PtrByte24 ImaSys::GetImageByte24Ptr() {
  ImagePtr<ImageByte24> tmp;
  return tmp.GetPtr();
}

int32_t ImaSys::SaveRawImage(const std::string& save_name, const void* data, uint32_t size) {
  int32_t ret = SaveRawIma(save_name, data, size);
  return ret;
}

int32_t ImaSys::SaveBmpImage(const std::string& save_name, const cv::Mat& img) {
  int32_t ret = SaveBmpIma(save_name, img);
  return ret;
}
}  // namespace image
