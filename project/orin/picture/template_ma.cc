#include <cassert>  //assert
#include <condition_variable>
#include <exception>  //static_assert
#include <iostream>
#include <iterator>
#include <mutex>  // std::mutex, std::lock_guard
#include <string>
#include <vector>

#include "base_context.h"
#include "common_log.h"
#include "log_printf.h"

// #include "template.h"
// undefined reference to `wm::Swap(int&, int&)'

/**
:编译器会对函数模板进行两次编译:
            | 在声明的地方对模板代码本身进行编译;
            | 在调用的地方对参数替换后的代码进行编译;

因此在工程里面如果只包含.h文件在链接时会出错,因为第一次编译只进行语法的检验,而不生成函数体,在main中再次调用模板函数时,
会无法在此文件中链接,因此还要把.CPP包含进来,同时在.cpp里包含.h文件,显然把.cpp改写成.hpp,符合平时书写习惯；
*/

void Save() {
  std::string src_pic{"./src/save_vc0_"};
  std::string tar_pic{"./dst/save_vc0_"};
  uint32_t src_size = 4746240;
  uint32_t src_front_size = 1920 * 24 * 2;
  uint32_t src_data_size = 1920 * 1208 * 2;
  uint32_t tar_size = 5986560;
  uint32_t tar_front_size = 1920 * 2;
  uint32_t from = 10;
  while (from < 100) {
    std::string src_name = src_pic;
    std::string tar_name = tar_pic;
    src_name += std::to_string(from) + ".raw";
    tar_name += std::to_string(from) + ".raw";
    FILE *src_f = fopen(src_name.data(), "r");
    FILE *dst_f = fopen(tar_name.data(), "w");
    if (src_f == nullptr) {
      PRINT_ERRNO("open %s failed", src_name.data());
      break;
    }
    if (src_f == nullptr) {
      PRINT_ERRNO("open %s failed", tar_name.data());
      break;
    }
    std::vector<uint8_t> src_buf(src_size, 0);
    std::vector<uint8_t> tar_buf(tar_size, 0);
    size_t rr = fread(src_buf.data(), 1, src_size, src_f);
    if (rr != src_size) {
      PRINT_ERRNO("read file %s %u %u failed\n", src_name.data(), rr, src_buf.size());
      break;
    }
    memcpy(tar_buf.data() + tar_front_size, src_buf.data() + src_front_size, src_data_size);
    if (fwrite(tar_buf.data(), tar_size, 1, dst_f) != 1) {
      PRINT_ERRNO("write file %s failed\n", tar_name.data());
      break;
    }
    from++;
    fclose(dst_f);
    fclose(src_f);
    dst_f = nullptr;
    src_f = nullptr;
  }
}
void Save_t() {
  std::string src_pic{"./src/save_vc0_"};
  std::string tar_pic{"./dst/save_vc0_"};
  uint32_t src_size = 4746240;
  uint32_t src_front_size = 1920 * 24 * 2;
  uint32_t src_data_size = 1920 * 1208 * 2;
  uint32_t tar_size = 5986560;
  uint32_t tar_front_size = 1920 * 2;
  uint32_t from = 10;
  while (from < 100) {
    std::string src_name = src_pic;
    std::string tar_name = tar_pic;
    src_name += std::to_string(from) + ".raw";
    tar_name += std::to_string(from) + ".raw";
    FILE *src_f = fopen(src_name.data(), "r");
    FILE *dst_f = fopen(tar_name.data(), "w");
    if (src_f == nullptr) {
      PRINT_ERRNO("open %s failed", src_name.data());
      break;
    }
    if (src_f == nullptr) {
      PRINT_ERRNO("open %s failed", tar_name.data());
      break;
    }
    std::vector<uint8_t> src_buf(src_size, 0);
    std::vector<uint8_t> tar_buf(tar_size, 0);
    size_t rr = fread(src_buf.data(), 1, src_size, src_f);
    if (rr != src_size) {
      PRINT_ERRNO("read file %s %u %u failed\n", src_name.data(), rr, src_buf.size());
      break;
    }
    memcpy(tar_buf.data(), src_buf.data(), src_size);
    if (fwrite(tar_buf.data(), tar_size, 1, dst_f) != 1) {
      PRINT_ERRNO("write file %s failed\n", tar_name.data());
      break;
    }
    from++;
    fclose(dst_f);
    fclose(src_f);
    dst_f = nullptr;
    src_f = nullptr;
  }
}
int main(int argc, char *argv[]) {
  try {
    Save_t();

  } catch (std::exception &err) {
    std::cout << "exception:" << err.what() << std::endl;
  } catch (...) {
    std::cout << "exception but do nothing" << std::endl;  // do nothing
    throw;                                                 //重新抛出
  }
  return 0;
}
