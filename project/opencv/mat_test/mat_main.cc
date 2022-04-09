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

void MatAttrs(Mat &img) {
  base_con::PrintFlag("opencv attrs");
  PRINT("src_img.flags = %d", img.flags);
  PRINT("img.dims = %d", img.dims);  //矩阵的维度
  PRINT("img.rows = %d", img.rows);
  PRINT("img.cols = %d", img.cols);
  PRINT("img.channels() = %d", img.channels());
  PRINT("img.empty() = %d", img.empty());
  PRINT("img.channels() = %d", img.channels());
  PRINT("img.size[0] = %d img.size[1] = %d", img.size[0], img.size[1]);
  PRINT("img.step[0] = %ld img.step[1] = %ld", img.step[0], img.step[1]);
  // MatSize size;
  // MatStep step;
  //数据内是一维数组，有个两元素，size[0]矩阵的行数，size[1]矩阵的列数
  // step[0]矩阵一行共多少字节，step[1]表示矩阵一个元素多少字节
  if (img.data) {
    PRINT("img.data = %ld", reinterpret_cast<long int>(img.data));
    PRINT("img.data[0] = %d data[1] = %d", img.data[0], img.data[1]);
  }
  if (img.u)  // UMatData* u;
    PRINT("u->refcount = %d", img.u->refcount);
}

void MatTest() {
  cv::Mat test;
  MatAttrs(test);
  cv::Mat test1(1080, 1920, CV_8UC1);
  MatAttrs(test1);
}
int main(int argc, char *argv[]) {
  base_con::PrintFlag("opencv test");

  PTS_INFO("i am zlog !!!");
  PRINT("i am C log !!!");

  try {
    MatTest();
  } catch (std::exception &err) {
    std::cout << "exception:" << err.what() << std::endl;
  } catch (...) {
    std::cout << "exception but do nothing" << std::endl;  // do nothing
    throw;                                                 //重新抛出
  }
  return 0;
}
