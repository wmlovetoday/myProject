#include <cassert>  //assert
#include <cstdint>
#include <exception>  //static_assert
#include <iostream>
#include <string>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

#include "base_context.h"
#include "image_sys.h"
#include "log_printf.h"

using namespace cv;
enum FileType { COM_AUTO = 1, COM_RAW8, COM_RAW16, COM_RAW24, COM_RAW32, COM_YUV422, COM_YUV420, COM_RGB };
static std::string file_name{};
static int32_t file_type{-1};
static int32_t convert_type{-1};
static uint32_t width{};
static uint32_t height{};
static uint32_t resolution{};

// void MatAttrs(Mat &img) {
//   base_con::PrintFlag("opencv attrs");
//   if (!img.empty()) {
//     PRINT("src_img.flags = %d", img.flags);
//     PRINT("img.total() = %d", img.total());
//     PRINT("img.dims = %d", img.dims);  //矩阵的维度
//     PRINT("img.rows = %d", img.rows);
//     PRINT("img.cols = %d", img.cols);
//     PRINT("img.channels() = %d", img.channels());
//     PRINT("img.channels() = %d", img.channels());
//     PRINT("img.size[0] = %d img.size[1] = %d", img.size[0], img.size[1]);
//     PRINT("img.step[0] = %ld img.step[1] = %ld", img.step[0], img.step[1]);
//     // MatSize size;
//     // MatStep step;
//     //数据内是一维数组，有个两元素，size[0]矩阵的行数，size[1]矩阵的列数
//     // step[0]矩阵一行共多少字节，step[1]表示矩阵一个元素多少字节
//     if (img.data) {
//       PRINT("img.data = %ld", reinterpret_cast<long int>(img.data));
//       PRINT("img.data[0] = %d data[1] = %d", img.data[0], img.data[1]);
//     }
//     if (img.u)  // UMatData* u;
//       PRINT("u->refcount = %d", img.u->refcount);
//   }
//   PRINT("img.empty() = %d", img.empty());
// }

// void MatTest() {
//   cv::Mat test;
//   MatAttrs(test);
//   cv::Mat test1(1080, 1920, CV_8UC1);
//   MatAttrs(test1);
// }
void Save(Mat &img) {
  PRINT(" 1 save to rgb");
  std::vector<int64_t> in = com::GetIntInput();
  if (!in.empty()) {
    if (in[0] == 1) {
      PRINT("save to rgb");
      if (!img.empty()) {
        std::string save_name = file_name + ".rgb";
        FILE *file = fopen(save_name.data(), "w+");
        // memcpy(file, img.data, img.total());
        fwrite(img.data, img.total() * img.channels(), 1, file);
        fclose(file);
      }
    }
    if (in[0] == 2) {
      PRINT("save to rgba");
      if (!img.empty()) {
        std::string save_name = file_name + ".rgba";
        uint32_t src_count = img.total() * 3;
        uint32_t dst_count = img.total() * 4;
        PRINT("SRC %d dst %d", src_count, dst_count);
        std::vector<char> mem(dst_count, 0);
        uint32_t y = 0;
        for (auto i = 0; i < src_count; i += 3) {
          mem[y] = img.data[i];
          mem[y + 1] = img.data[i + 1];
          mem[y + 2] = img.data[i + 2];
          mem[y + 3] = 0;
          y += 4;
        }
        FILE *file = fopen(save_name.data(), "w+");
        fwrite(mem.data(), dst_count, 1, file);
        fclose(file);
      }
    }
  }
}
void Display() {
  if (file_type == COM_AUTO) {
    // Mat dst_img = imread(file_name.data());
    // imshow(WINDOW, dst_img);
    // int8_t key = static_cast<int8_t>(waitKey(0));
    // if (key == 's') {
    //   std::string w_name = file_name + ".bmp";
    //   imwrite(w_name.data(), dst_img);
    // }
    FILE *file = fopen(file_name.c_str(), "r");
    fseek(file, 0, SEEK_END);
    resolution = ftell(file);
    width = 1;
    height = resolution;
    PRINT("FILE SIZE %d %d %d", resolution, width, height);
    fclose(file);
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
  } else if (file_type == COM_RGB) {
    resolution = width * height * 3;
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
  cv::Mat dst_img{};
  cv::Mat src_img{};

  dst_img.create(height, width, CV_8UC3);
  if (file_type == COM_RAW8) {
    src_img = cv::Mat(height, width, CV_8UC1, static_cast<void *>(const_cast<char *>(mem.data())), width);
    cv::cvtColor(src_img, dst_img, COLOR_BayerBG2BGR, 3);
  } else if (file_type == COM_YUV422) {
    src_img = cv::Mat(height, width, CV_8UC2, static_cast<void *>(const_cast<char *>(mem.data())), width * 2);
    cv::cvtColor(src_img, dst_img, CV_YUV2BGR_YUYV, 3);
  } else if (file_type == COM_YUV420) {
    src_img = cv::Mat(height * 3 / 2, width, CV_8UC2, static_cast<void *>(const_cast<char *>(mem.data())), width);
    cv::cvtColor(src_img, dst_img, CV_YUV2RGB_YV12, 3);
  }
  Mat rsz_img = Mat::zeros(height / 2, width / 2, CV_8UC3);
  resize(dst_img, rsz_img, rsz_img.size());
  // imshow("test", dst_img);
  imshow("test", rsz_img);
  cv::waitKey(0);

  // MatAttrs(src_img);
  // MatAttrs(dst_img);
  // Save(dst_img);
  // if (file_type == COM_RGB) {
  //   dst_img = cv::Mat(height, width, CV_8UC3, static_cast<void *>(const_cast<char *>(mem.data())), width * 3);
  //   imshow("test", dst_img);
  //   cv::waitKey(0);
  // }
  fclose(fp);
}
// int main(int argc, char *argv[]) {
//   base_con::PrintFlag("opencv test");

//   PRINT("i am C log !!!");
//   if (argc < 6) {
//     PRINT("/////////// must input arguments //////////");
//     PRINT(" ./mat file, filetype, width, hight, convert type");
//     // PRINT(" ./mat lena.jpg 1 0 0 0");
//     PRINT(" ./mat 1080p_raw8.raw8 2 1920 1080 1");
//     PRINT(" ./mat sony.raw24 4 1920 1080 1");
//     PRINT(" ./mat 1920_1080_4bytes_rggb.raw32 5 1920 1080 1");
//     PRINT(" ./mat 1280_720.yuv422 6 1280 720 1");
//     PRINT(" ./mat 176_144.yuv420 7 176 144 1");
//     PRINT(" ./mat 1080p_raw8.raw8.rgb 8 1920 1080 1");

//     return -1;
//   }
//   file_name = argv[1];
//   file_type = std::stoi(argv[2]);
//   width = std::stoi(argv[3]);
//   height = std::stoi(argv[4]);
//   convert_type = std::stoi(argv[5]);
//   try {
//     // MatTest();
//     Display();
//   } catch (std::exception &err) {
//     std::cout << "exception:" << err.what() << std::endl;
//   } catch (...) {
//     std::cout << "exception but do nothing" << std::endl;  // do nothing
//     throw;                                                 //重新抛出
//   }
//   return 0;
// }

int main(int argc, char *argv[]) {
  file_name = argv[1];
  width = 3840;
  height = 2160;
  resolution = width * height * 3 / 2;
  std::vector<char> mem(resolution, 0);
  FILE *fp;
  if ((fp = fopen(file_name.c_str(), "r")) == nullptr) {
    printf("open %s failed %s:%d!", file_name.data(), strerror(errno), errno);
  }
  size_t len = fread(mem.data(), 1, resolution, fp);
  if (len != resolution) {
    printf("need read %d, but read %ld!", resolution, len);
    exit(0);
  }
  cv::Mat dst_img{};
  cv::Mat src_img{};

  dst_img.create(height, width, CV_8UC3);
  src_img = cv::Mat(height * 3 / 2, width, CV_8UC2, static_cast<void *>(const_cast<char *>(mem.data())), width);
  cv::cvtColor(src_img, dst_img, CV_YUV2RGB_YV12, 3);
  Mat rsz_img = Mat::zeros(height / 2, width / 2, CV_8UC3);
  resize(dst_img, rsz_img, rsz_img.size());
  imshow("test", rsz_img);
  cv::waitKey(0);
}
