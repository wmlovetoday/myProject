#ifndef SRC_IMAGE_H
#define SRC_IMAGE_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

#include "image_base.h"
#include "image_data_defs.h"
// #include "image_sys.h"

namespace image {

int32_t SaveBmpIma(const std::string& save_name, const cv::Mat& img);
int32_t SaveRawIma(const std::string& save_name, const void* data, uint32_t size);

class ImageUi {
 public:
  void TrackbarInit();
  void CreateTrackbar(const std::string& win_name);
  void SaveImage(const cv::Mat& dst_img, const void* img, uint32_t size, const std::string ima_type = ".bmp");
  void SaveVideo(const std::string& video_name, uint16_t width, uint16_t height, const cv::Mat& dst_img);
  inline SaveImageTrackbar* GetPicTrackbar() { return &pic_tracbar_; }
  inline SaveVideoTrackbar* GetVideoTrackbar() { return &video_tracbar_; }

 protected:
  SaveImageTrackbar pic_tracbar_;
  SaveVideoTrackbar video_tracbar_;
  SavePicTypeTrackbar save_pic_type_tracbar_;
  cv::VideoWriter video_;
  bool new_video_{true};
};

class ImageByte8 : public ImaBase, public ImageUi {
 public:
  void Display(uint16_t width, uint16_t height, const void* memory, const std::string name);

  ~ImageByte8();

 private:
};

class ImageByte24 : public ImaBase, public ImageUi {
 public:
  void Display(uint16_t width, uint16_t height, const void* memory, const std::string name);

  ~ImageByte24();

 private:
  std::vector<uint8_t> low_mem8_{};
  std::vector<uint8_t> mid_mem8_{};
  std::vector<uint8_t> hig_mem8_{};
  cv::Mat low_raw8_;
  cv::Mat mid_raw8_;
  cv::Mat hig_raw8_;
  cv::Mat low_rgb24_;
  cv::Mat mid_rgb24_;
  cv::Mat hig_rgb24_;
  cv::Mat ldrReinhard_;
  cv::Mat hdr32_;
  cv::Ptr<cv::TonemapReinhard> tonemapReinhard_;
};

}  // namespace image
#endif