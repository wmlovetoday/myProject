#include <cassert>  //assert
#include <cstdint>
#include <exception>  //static_assert
#include <iostream>
#include <string>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

#include "base_context.h"
#include "common_log.h"
#include "image_sys.h"
#include "log_printf.h"

using namespace cv;
enum FileType { COM_AUTO = 1, COM_RAW8, COM_RAW16, COM_RAW24, COM_RAW32, COM_YUV422, COM_YUV420 };

static std::string file_name{"1080p_raw8.raw8"};
static uint32_t width{1920};
static uint32_t height{1080};
static uint32_t resolution{1920 * 1080};

#define WINDOW "window_"

static void Display() {
  resolution = width * height;
  std::vector<char> mem(resolution, 0);
  FILE *fp;
  if ((fp = fopen(file_name.c_str(), "r")) == nullptr) {
    PRINT("open %s failed %s:%d!", file_name.data(), strerror(errno), errno);
  }
  size_t len = fread(mem.data(), 1, resolution, fp);
  if (len != resolution) {
    PRINT("need read %d, but read %ld!", resolution, len);
    exit(0);
  }

  std::string win_name_{"win"};

  /*int createTrackbar(
     const String &trackbarname, const String &winname, int *value, int count, TrackbarCallback onChange = 0,
     void *userdata = 0);*/
  cv::TrackbarCallback callback = [](int pos, void *userdata) { PRINT("POS = %d", pos); };
  int ret_value = 2;
  int max_value = 5;

  cv::Mat dst_img_;
  dst_img_.create(height, width, CV_8UC3);
  cv::Mat src_img(height, width, CV_8UC1, static_cast<void *>(const_cast<char *>(mem.data())), width);

  cv::cvtColor(src_img, dst_img_, COLOR_BayerBG2BGR, 3);
  cv::namedWindow(win_name_, WINDOW_NORMAL | CV_GUI_NORMAL);
  imshow(win_name_, dst_img_);
  cv::createTrackbar("track test", win_name_, &ret_value, max_value, callback);
  int8_t key = static_cast<int8_t>(cv::waitKey(0));
  fclose(fp);
  cv::destroyWindow(win_name_);
}

int main(int argc, char *argv[]) {
  base_con::PrintFlag("opencv test");
  try {
    Display();
  } catch (std::exception &err) {
    std::cout << "exception:" << err.what() << std::endl;
  } catch (...) {
    std::cout << "exception but do nothing" << std::endl;  // do nothing
    throw;                                                 //重新抛出
  }
  return 0;
}
