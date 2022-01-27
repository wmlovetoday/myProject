
#include <iostream>
#include <string>

namespace base_con {

void PrintFlag(std::string &flag) {
  std::cout << "------------------ " << flag << " ------------------"
            << std::endl;
  std::cout << "" << std::endl;
}
void PrintFlag(char *flag) {
  std::cout << "------------------ " << flag << " ------------------"
            << std::endl;
  std::cout << "" << std::endl;
}
} // namespace base_con