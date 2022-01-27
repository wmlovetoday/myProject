#ifndef INC_PT_SEN_COMMON_LOG_
#define INC_PT_SEN_COMMON_LOG_

#include <unistd.h>
#include <cassert>
#include <cerrno>
#include <cstdint>
#include <cstdio>
#include <exception>
#include <mutex>
#include <string>
#include <utility>
#include <vector>

#include "fmt/format.h"
#include "zlog.h"

namespace pts {
/** use define,Convenient and quick if change other log,
 * especially replaced by MW_INFO
 * because zlog is c library, use format library format to string.*/

/** user define log */
#define PTS_U_INFO(cat, ...)                    \
  {                                             \
    std::string xxx = fmt::format(__VA_ARGS__); \
    if (cat) zlog_info(cat, "%s", xxx.data());  \
  }

/** sdk default log */
#define PTS_INFO(...)                                                          \
  {                                                                            \
    std::string xxx = fmt::format(__VA_ARGS__);                                \
    if (pts::com_log_handle) zlog_info(pts::com_log_handle, "%s", xxx.data()); \
  }
#define PTS_ERROR(...)                                                          \
  {                                                                             \
    std::string xxx = fmt::format(__VA_ARGS__);                                 \
    if (pts::com_log_handle) zlog_error(pts::com_log_handle, "%s", xxx.data()); \
  }
#define PTS_DEBUG(...)                                                          \
  {                                                                             \
    std::string xxx = fmt::format(__VA_ARGS__);                                 \
    if (pts::com_log_handle) zlog_debug(pts::com_log_handle, "%s", xxx.data()); \
  }
#define PTS_WARN(...)                                                          \
  {                                                                            \
    std::string xxx = fmt::format(__VA_ARGS__);                                \
    if (pts::com_log_handle) zlog_warn(pts::com_log_handle, "%s", xxx.data()); \
  }

/** no format log */
#define COM_LOG(format, ...) \
  if (pts::com_log_handle) zlog_info(pts::com_log_handle, format, ##__VA_ARGS__)

/** no format,output data block */
#define COM_BUF(format, ...) \
  if (pts::com_log_handle) hzlog_info(pts::com_log_handle, format, ##__VA_ARGS__)

constexpr int32_t kLogSuccess = 0;
constexpr int32_t kLogFailed = -1;

class ComLog {
 public:
  static ComLog *GetLogInstance() {
    static ComLog ObjLog; /** zlog_init can only execute once */
    return &ObjLog;
  }
  /**
   * @brief get user define log category,
   * @brief category can output log to different log file
   * @param[in] category : log category
   * @param[in] out : output log file with dir
   * @param[in] size : one log file size(MB)
   * @param[in] count : count of log file
   * @param[out] zlog_category_t* : log category,
   * @return<pre>
   * if success return log category,
   * otherwise throw exception. </pre>
   */

  zlog_category_t *GetCategory(
      const std::string &category, const std::string &out, uint32_t size = 100, uint32_t count = 10) {
    if (category.empty() || out.empty() || (size == 0) || (count == 0)) {
      printf("arguments illegal\n");
      return nullptr;
    }
    std::lock_guard<std::mutex> lck(log_mut);
    return (ComLogConfig(category, out, size, count));
  }

  int32_t PutCategory(const std::string &category, const std::string &out) {
    int32_t ret = kLogFailed;
    if ((category.empty()) && (out.empty())) {
      printf("ComLog: argment illegal\n");
      return ret;
    }
    std::lock_guard<std::mutex> lck(log_mut);
    return (RemoveComLogConfig(category, out));
  }

  inline zlog_category_t *GetHandler() { return log_handle_; }
  ~ComLog() {
    zlog_fini();
    printf("ComLog: log finit success\n");
  }

 private:
  std::mutex log_mut{};
  std::string default_cat_{"com_sys"};
  zlog_category_t *log_handle_{nullptr};
  timespec tv{};
  std::string global_con_{"[global]\nbuffer min = 64\n"};
  std::string rotate_file_{};
  std::string others_con_{
      "[formats]\nnormal = \"%d(%T) %us [%V] %f:%L %m %n\"\n\n[rules]\ncom_sys.* >stdout; "
      "normal"};
  std::string configure_{};
  /*   std::string configure_{
        "[global]\nbuffer min = 64\n[formats]\nnormal = \"%d(%T) %us [%V] %f:%L %m %n\"\n\n[rules]\ncom_sys.* >stdout; "
        "normal"}; */
  std::vector<std::string> rules_vec_{};
  // std::string configure_{"[formats]\nnormal = \"%d(%T) %us %m [%f:%L]%n\"\n\n[rules]\n*.* \"com_err.log\";normal"};
  ComLog() {
    // zlog_profile();
    uint32_t try_count = 10;
    while (try_count--) {
      int32_t ret = clock_gettime(CLOCK_REALTIME, &tv);
      if (0 == ret) {
        uint32_t ns = static_cast<uint32_t>(tv.tv_nsec & 0xffffffUL);
        configure_.clear();
        configure_ = global_con_ + "rotate lock file = /tmp/zlog.lock_" + std::to_string(ns) + "\n" + others_con_;
        break;
      }
      usleep(10);
    }
    uint32_t ret = zlog_init(configure_.data());
    if (ret) {
      throw std::runtime_error{"ComLog:log init failed"};
    }
    log_handle_ = zlog_get_category(default_cat_.data());
    if (log_handle_ == nullptr) {
      throw std::runtime_error{"ComLog:log category:sys failed"};
    }
    zlog_info(log_handle_, "ComLog: log init success");
  }

  zlog_category_t *ComLogConfig(const std::string &category, const std::string &out, uint32_t size, uint32_t count) {
    if ((category.empty()) || (out.empty()) || (size == 0) || (count == 0)) {
      throw std::runtime_error{"ComLog: argment illegal"};
    }

    std::string tmp{
        category + std::string{".* \""} + out + std::string{"\","} + std::to_string(size) + std::string("M * ") +
        std::to_string(count) + std::string{"; normal"}};
    rules_vec_.push_back(tmp);
    std::string cat_tmp{configure_};
    for (const auto &i : rules_vec_) {
      cat_tmp += std::string{"\n"};
      cat_tmp += i;
    }
    int32_t ret = zlog_reload(cat_tmp.data());
    if (ret) {
      RemoveComLogConfig(category, out);
      printf("zlog reload failed %s:%d", strerror(errno), errno);
      throw std::runtime_error{"ComLog: log reload failed"};
    }
    zlog_category_t *handler = zlog_get_category(category.data());
    if (handler == nullptr) {
      RemoveComLogConfig(category, out);
      throw std::runtime_error{"ComLog: log category:sys failed"};
    }
    return handler;
  }

  int32_t RemoveComLogConfig(const std::string &category, const std::string &out) {
    int32_t ret = kLogFailed;
    std::string tmp = category + std::string{".* \""} + out; /**must same with rules_vec_*/
    for (uint32_t i = 0; i < rules_vec_.size(); i++) {
      if (strncmp(rules_vec_.at(i).data(), tmp.data(), tmp.size()) == 0) {
        rules_vec_.erase(rules_vec_.begin() + i);
        printf("ComLog: remove %s %s %d\n", tmp.data(), __func__, __LINE__);
        return kLogSuccess;
      }
    }
    printf("ComLog: not find %s %s %d\n", tmp.data(), __func__, __LINE__);
    return ret;
  }
};

extern class ComLog *com_log;
extern zlog_category_t *com_log_handle;
}  // namespace pts
#endif