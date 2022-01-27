#include "common_interface.h"

constexpr uint32_t MAJOR_VERSION = 1;
constexpr uint32_t MINOR_VERSION = 1;
constexpr uint32_t REVERSION = 1;
namespace pts
{
int32_t common::SdkVersion(std::string& version)
{
    try
    {
        version.assign("V");
        std::string tmp{
            std::to_string(MAJOR_VERSION) + std::string(".") + std::to_string(MINOR_VERSION) + std::string(".") +
            std::to_string(REVERSION)};
        version += tmp;
        return 0;
    }
    catch (std::exception& err)
    {
        printf("catch exception %s %s %d", err.what(), __func__, __LINE__);
        return -1;
    }
}
}  // namespace pts