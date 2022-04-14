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

static std::string file_name{};
static int32_t file_type{-1};
static int32_t convert_type{-1};
static uint32_t width{};
static uint32_t height{};
static uint32_t resolution{};

#define WINDOW "window_"

static void Display() {
  if (file_type == COM_AUTO) {
    Mat dst_img = imread(file_name.data());
    imshow(WINDOW, dst_img);
    int8_t key = static_cast<int8_t>(waitKey(0));
    if (key == 's') {
      std::string w_name = file_name + ".bmp";
      imwrite(w_name.data(), dst_img);
    }
    exit(0);
  }
  if (file_type == COM_RAW8) {
    resolution = width * height;
  } else if (file_type == COM_RAW16) {
    resolution = width * height * 2;
  } else if (file_type == COM_RAW24) {
    resolution = width * height * 3;
  } else if (file_type == COM_RAW32) {
    resolution = width * height * 4;
  } else if (file_type == COM_YUV422) {
    resolution = width * height * 2;
  } else if (file_type == COM_YUV420) {
    resolution = width * height * 3 / 2;
  }
  std::vector<char> mem(resolution, 0);
  FILE *fp;
  if (file_type == COM_RAW32) {
    std::vector<char> mem_t(width * height * 4, 0);
    if ((fp = fopen(file_name.c_str(), "r")) == nullptr) {
      PRINT("open %s failed %s:%d!", file_name.data(), strerror(errno), errno);
    }
    size_t len = fread(mem_t.data(), 1, resolution, fp);
    if (len != resolution) {
      PRINT("need read %d, but read %ld!", resolution, len);
      exit(0);
    }
    uint32_t j = 0;
    for (auto i = 0; i < resolution; i += 4) {
      mem[j] = mem_t[i];
      mem[j + 1] = mem_t[i + 1];
      mem[j + 2] = mem_t[i + 2];
      j += 3;
    }
  } else {
    if ((fp = fopen(file_name.c_str(), "r")) == nullptr) {
      PRINT("open %s failed %s:%d!", file_name.data(), strerror(errno), errno);
    }
    size_t len = fread(mem.data(), 1, resolution, fp);
    if (len != resolution) {
      PRINT("need read %d, but read %ld!", resolution, len);
      exit(0);
    }
  }
  using namespace image;
  if ((file_type == COM_RAW8) || (file_type == COM_YUV422) || (file_type == COM_YUV420)) {
    auto cam = ImaSys::GetImageByte8Ptr();
    cam->SetWaitMs(0);
    if (file_type == COM_RAW8) {
      cam->SetConvertType(image::ConvertType::kBayer2BG);
    }
    if (file_type == COM_YUV422) {
      cam->SetConvertType(image::ConvertType::kYUV2BG);
    }
    if (file_type == COM_YUV420) {
      cam->SetConvertType(image::ConvertType::kYV122RG);
    }
    cam->Display(width, height, mem.data(), "IMA");
    cam->Display(width, height, mem.data(), "IMA");
  }
  if ((file_type == COM_RAW24) || (file_type == COM_RAW32)) {
    auto cam = ImaSys::GetImageByte24Ptr();
    cam->SetWaitMs(0);
    cam->SetConvertType(image::ConvertType::kBayer2GR);
    cam->Display(width, height, mem.data(), "IMA");
    cam->Display(width, height, mem.data(), "IMA");
  }
  fclose(fp);
}

int main(int argc, char *argv[]) {
  base_con::PrintFlag("opencv test");

  PTS_INFO("i am zlog !!!");
  PRINT("i am C log !!!");
  if (argc < 6) {
    PRINT("/////////// must input arguments //////////");
    PRINT(" ./display file, filetype, width, hight, convert type");
    PRINT(" ./display lena.jpg 1 0 0 0");
    PRINT(" ./display 1080p_raw8.raw8 2 1920 1080 1");
    PRINT(" ./display sony.raw24 4 1920 1080 1");
    PRINT(" ./display 1920_1080_4bytes_rggb.raw32 5 1920 1080 1");
    PRINT(" ./display 1280_720.yuv422 6 1280 720 1");
    PRINT(" ./display 176_144.yuv420 7 176 144 1");
    PRINT(
        "   file type : COM_AUTO(1), COM_RAW8(2), COM_RAW16(3), COM_RAW24(4),COM_RAW24(5), COM_YUV422(6), "
        "COM_YUV420(7)");
    return 1;
  } else {
    file_name = argv[1];
    file_type = std::stoi(argv[2]);
    width = std::stoi(argv[3]);
    height = std::stoi(argv[4]);
    convert_type = std::stoi(argv[5]);
    PTS_INFO(
        " file : {}; filetype : {}; width : {}; hight : {}; convert type {}", file_name, file_type, width, height,
        convert_type);
  }

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
