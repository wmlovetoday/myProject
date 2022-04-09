#ifndef INS_GEN_API_GEN_POINTER_H
#define INS_GEN_API_GEN_POINTER_H

#include "enumeration.h"

namespace sensor {
namespace gen_api {

using EnumerationPtr = Pointer<Enumeration>;

// template <class T, class B>
// inline bool IsReadable(const sensor::gen_api::Pointer<T, B>& ptr) {
//   return ptr.IsValid() && IsReadable(ptr->GetAccessMode());
// }

inline bool IsReadable(const sensor::gen_api::Enumeration &ptr) {
  return ptr->IsReadable();
}

inline bool IsWritable(const sensor::gen_api::Enumeration &ptr) {
  return ptr->IsWritable();
}

inline bool IsImplemented(const sensor::gen_api::Enumeration &ptr) {
  return ptr->IsImplemented();
}

class Pointer {
 public:
  bool IsAvailable();
  bool IsWritable();
  bool IsReadable();
  bool IsImplemented();
  SetValue();
  GetValue();
  GetMax();
  GetMin();

 private:
};

class SensorPtr : public Pointer {};

}  // namespace gen_api
}  // namespace sensor

#endif