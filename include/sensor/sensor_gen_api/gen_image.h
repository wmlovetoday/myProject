#ifndef INS_GEN_API_GEN_IMAGE_H
#define INS_GEN_API_GEN_IMAGE_H
namespace sensor {
namespace gen_api {

class Image {
  uint64_t GetID();       // buffer id
  uint64_t GetFrameID();  // image id
  bool IsIncomplete();    // lost
  uint64_t GetTLPixelFormat();
  void* GetData();
  size_t GetValidPayloadSize();  //
  void* GetPrivateData();
  size_t GetWidth();
  size_t GetHeight();
  size_t GetXOffset();
  size_t GetYOffset();
  uint64_t GetTimeStamp();
  ImageStatus GetImageStatus();
  // <todo>
  // lost pocket id list?
};

}  // namespace gen_api
}  // namespace sensor

#endif