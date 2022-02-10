#ifndef INC_DDS_INTERFACE_H_
#define INC_DDS_INTERFACE_H_

namespace dds {

class Ddsi {
 public:
  virtual int32_t Read(void* buf, uint32_t len, uint32_t wait_ms, const std::string& topic) = 0;
  virtual int32_t Write(const void* buf, uint32_t len, const std::string& topic) = 0;
  virtual ~Ddsi(){};

 protected:
 private:
};

}  // namespace dds
#endif