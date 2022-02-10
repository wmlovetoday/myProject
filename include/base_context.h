
#ifndef INC_BASE_CON_
#define INC_BASE_CON_

#include <iostream>
#include <string>

namespace base_con {

void PrintFlag(std::string &flag) {
  std::cout << '\n'
            << "------------------ " << flag << " ------------------" << '\n'
            << std::endl;
  std::cout << "" << std::endl;
}
void PrintFlag(const char *flag) {
  std::cout << '\n'
            << "------------------ " << flag << " ------------------" << '\n'
            << std::endl;
}
} // namespace base_con
#endif