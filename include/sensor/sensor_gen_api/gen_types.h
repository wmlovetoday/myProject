#ifndef INS_GEN_API_GEN_TYPES_H
#define INS_GEN_API_GEN_TYPES_H
namespace sensor {
namespace gen_api {

enum EAccessMode {
  NI = 1,               /** Not implemented*/
  NA,                   /** Not available*/
  WO,                   /** Write Only*/
  RO,                   /** Read Only*/
  RW,                   /** Read and Write*/
  kUndefinedAccesMode,  /** Object is not yet initialized*/
  kCycleDetectAccesMode /** used internally for AccessMode cycle detection*/
};
enum TriMode {
  kTriMaster = 1,
  kTriSlaveSoft,
  kTriSlaveExternal,
  kTriSlaveFree
};

enum ImageFormat {
  kRaw8 = 1,
  kRaw16,
  kRaw24,
  kRaw12Pwl,
  kRaw12Tone,
  kRgb8,
  kJpeg
};
}  // namespace gen_api
}  // namespace sensor

#endif