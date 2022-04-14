#include "image.h"
#include "com_interface.h"

namespace image {

using namespace cv;

int32_t SaveBmpIma(const std::string &save_name, const cv::Mat &img) {
  if (save_name.empty()) {
    return kImaArgsIllegal;
  }
  std::string w_name = save_name + ".bmp";
  cv::imwrite(w_name.data(), img);
  return kImaSuccess;
}

int32_t SaveRawIma(const std::string &save_name, const void *data, uint32_t size) {
  if (save_name.empty()) {
    return kImaArgsIllegal;
  }
  FILE *fp = fopen(save_name.data(), "w+");
  if (fp < 0) {
    return kImaFailed;
  } else {
    size_t w_size = fwrite(data, 1, size, fp);
    if (size != w_size) {
      PRINT_ERRNO("save %s error\n", save_name.data());
      fclose(fp);
      return kImaFailed;
    }
  }
  fclose(fp);
  return kImaSuccess;
}

void ImageUi::SaveImage(const cv::Mat &dst_img, const void *img, uint32_t size, const std::string ima_type) {
  if (pic_tracbar_.pic_dir.empty()) {
    char dir[32];
    com::GetTimeLogo(&dir);
    pic_tracbar_.pic_dir = "picture_" + std::string{dir};
    com::CreateDir(pic_tracbar_.pic_dir.data());
  }

  std::string name = pic_tracbar_.pic_dir + "/" + std::to_string(pic_tracbar_.pic_num) + ima_type;
  // PRINT("save : %s", name.data());
  if (ima_type == std::string{".raw"}) {
    SaveRawIma(name, img, size);
  } else {
    cv::imwrite(name, dst_img);
  }
  if (pic_tracbar_.trackbar_value == static_cast<int32_t>(SavePicMode::kSingle)) {
    pic_tracbar_.trackbar_value = 0;
  }
  pic_tracbar_.pic_num++;
}
void ImageUi::SaveVideo(
    const std::string &video_name, uint16_t width, uint16_t height, uint16_t fps, const cv::Mat &dst_img) {
  if (video_tracbar_.pic_dir.empty()) {
    char dir[32];
    com::GetTimeLogo(&dir);
    video_tracbar_.pic_dir = "video_" + std::string{dir};
    com::CreateDir(video_tracbar_.pic_dir.data());
    std::string name = video_tracbar_.pic_dir + "/" + video_name + ".avi";
    // PRINT("SAVE %s", name.data());
    cv::VideoWriter vid(name, CV_FOURCC('M', 'J', 'P', 'G'), fps, CvSize(width, height));
    video_ = vid;
  }
  video_.write(dst_img);
}

void ImageUi::TrackbarInit() {
  pic_tracbar_.trackbar_value = static_cast<int32_t>(SavePicMode::kDefault);
  pic_tracbar_.trackbar_value_max = static_cast<int32_t>(SavePicMode::kContinue);
  pic_tracbar_.pic_num = 0;

  video_tracbar_.trackbar_value = static_cast<int32_t>(EnableMode::kOFF);
  video_tracbar_.trackbar_value_max = static_cast<int32_t>(EnableMode::kON);

  save_pic_type_tracbar_.trackbar_value = static_cast<int32_t>(SavePicType::kBmp);
  save_pic_type_tracbar_.trackbar_value_max = static_cast<int32_t>(SavePicType::kJpeg);

  /*int createTrackbar(
      const String &trackbarname, const String &winname, int *value, int max_count, TrackbarCallback onChange = 0,
      void *userdata = 0);*/
}
void ImageUi::CreateTrackbar(const std::string &win_name) {
  createTrackbar(
      save_pic_context.data(), win_name, &pic_tracbar_.trackbar_value, pic_tracbar_.trackbar_value_max, 0, 0);
  createTrackbar(
      save_video_context.data(), win_name, &video_tracbar_.trackbar_value, video_tracbar_.trackbar_value_max, 0, 0);
  createTrackbar(
      save_pic_type_context.data(), win_name, &save_pic_type_tracbar_.trackbar_value,
      save_pic_type_tracbar_.trackbar_value_max, 0, 0);
}

void ImageByte8::Display(uint16_t width, uint16_t height, const void *memory, const std::string name) {
  if (!init_) {
    dst_img_.create(height, width, CV_8UC3);
    cv::namedWindow(win_name_, WINDOW_NORMAL | CV_GUI_NORMAL);
    width_ = width;
    height_ = height;
    if ((type_ == ConvertType::kBayer2BG) || (type_ == ConvertType::kBayer2GB) || (type_ == ConvertType::kBayer2RG) ||
        (type_ == ConvertType::kBayer2GR)) {
      size_ = width * height;
    } else if ((type_ == ConvertType::kYUV2BG) || (type_ == ConvertType::kYUV2RG)) {
      size_ = width * height * 2;
    } else if ((type_ == ConvertType::kYV122RG) || (type_ == ConvertType::kYV122BG)) {
      size_ = width * height * 3 / 2;
    }

    TrackbarInit();
    init_ = true;
  }

  if (init_) {
    cv::Mat src_img;
    if (type_ == ConvertType::kBayer2BG) {
      cv::Mat tmp_img(height, width, CV_8UC1, const_cast<void *>(memory), width);
      src_img = tmp_img;
      cv::cvtColor(src_img, dst_img_, COLOR_BayerBG2BGR, 3);
    } else if (type_ == ConvertType::kBayer2GB) {
      cv::Mat tmp_img(height, width, CV_8UC1, const_cast<void *>(memory), width);
      src_img = tmp_img;
      cv::cvtColor(src_img, dst_img_, COLOR_BayerGB2BGR, 3);
    } else if (type_ == ConvertType::kBayer2RG) {
      cv::Mat tmp_img(height, width, CV_8UC1, const_cast<void *>(memory), width);
      src_img = tmp_img;
      cv::cvtColor(src_img, dst_img_, COLOR_BayerRG2BGR, 3);
    } else if (type_ == ConvertType::kBayer2GR) {
      cv::Mat tmp_img(height, width, CV_8UC1, const_cast<void *>(memory), width);
      src_img = tmp_img;
      cv::cvtColor(src_img, dst_img_, COLOR_BayerGR2BGR, 3);
    } else if (type_ == ConvertType::kYUV2BG) {
      cv::Mat tmp_img(height, width, CV_8UC2, const_cast<void *>(memory), width * 2);
      src_img = tmp_img;
      cv::cvtColor(src_img, dst_img_, CV_YUV2BGR_YUYV, 3);
    } else if (type_ == ConvertType::kYUV2RG) {
      cv::Mat tmp_img(height, width, CV_8UC2, const_cast<void *>(memory), width * 2);
      src_img = tmp_img;
      cv::cvtColor(src_img, dst_img_, CV_YUV2RGB_UYVY, 3);
    } else if (type_ == ConvertType::kYV122RG) {
      cv::Mat tmp_img(height * 3 / 2, width, CV_8UC2, const_cast<void *>(memory), width);
      src_img = tmp_img;
      cv::cvtColor(src_img, dst_img_, CV_YUV2RGB_YV12, 3);
    } else if (type_ == ConvertType::kYV122BG) {
      cv::Mat tmp_img(height * 3 / 2, width, CV_8UC2, const_cast<void *>(memory), width);
      src_img = tmp_img;
      cv::cvtColor(src_img, dst_img_, CV_YUV2BGR_YV12, 3);
    } else {
      PRINT(
          " %d current convert out of %d ~ %d", static_cast<int32_t>(type_),
          static_cast<int32_t>(ConvertType::kBayer2BG), static_cast<int32_t>(ConvertType::kYV122BG));
      return;
    }
    imshow(win_name_, dst_img_);
    CreateTrackbar(win_name_);
    if (pic_tracbar_.trackbar_value != static_cast<int32_t>(SavePicMode::kDefault)) {
      SaveImage(dst_img_, memory, size_, save_pic_type_);
    }
    if (video_tracbar_.trackbar_value == static_cast<int32_t>(EnableMode::kON)) {
      if (new_video_) {
        video_tracbar_.pic_dir.clear();
        new_video_ = false;
      }
      SaveVideo(win_name_, width_, height_, video_fps, dst_img_);
    } else {
      new_video_ = true;
    }
    if (static_cast<int32_t>(save_type_) != save_pic_type_tracbar_.trackbar_value) {
      SetSavePicType(static_cast<SavePicType>(save_pic_type_tracbar_.trackbar_value));
    }

    int8_t key = static_cast<int8_t>(cv::waitKey(wait_ms_));
    if ((key == 's') || (key == 'S')) {
      pic_tracbar_.trackbar_value = static_cast<int32_t>(SavePicMode::kSingle);
      SaveImage(dst_img_, memory, size_, save_pic_type_);
    }
  }
}
ImageByte8::~ImageByte8() {
  if (init_) {
    cv::destroyWindow(win_name_);
  }
}
void ImageByte24::Display(uint16_t width, uint16_t height, const void *memory, const std::string name) {
  if (!init_) {
    low_mem8_.resize(width * height);
    mid_mem8_.resize(width * height);
    hig_mem8_.resize(width * height);
    low_rgb24_.create(height, width, CV_8UC3);
    mid_rgb24_.create(height, width, CV_8UC3);
    hig_rgb24_.create(height, width, CV_8UC3);
    hdr32_.create(height, width, CV_32FC3);
    tonemapReinhard_ = createTonemapReinhard(1.5, 0, 0, 0);
    cv::namedWindow(win_name_, WINDOW_NORMAL | CV_GUI_NORMAL);
    width_ = width;
    height_ = height;
    size_ = width * height * 3;
    TrackbarInit();
    init_ = true;
  }

  if (init_) {
    for (int i = 0; i < height * width; i++) {
      uint32_t t = i * 3;
      low_mem8_[i] = *((uint8_t *)(memory) + t);
      mid_mem8_[i] = *((uint8_t *)(memory) + t + 1);
      hig_mem8_[i] = *((uint8_t *)(memory) + t + 2);
    }
    low_raw8_ = cv::Mat(height, width, CV_8UC1, const_cast<uint8_t *>(low_mem8_.data()), width);
    mid_raw8_ = cv::Mat(height, width, CV_8UC1, const_cast<uint8_t *>(mid_mem8_.data()), width);
    hig_raw8_ = cv::Mat(height, width, CV_8UC1, const_cast<uint8_t *>(hig_mem8_.data()), width);

    if (type_ == ConvertType::kBayer2BG) {
      cv::cvtColor(low_raw8_, low_rgb24_, COLOR_BayerBG2BGR, 3);
      cv::cvtColor(mid_raw8_, mid_rgb24_, COLOR_BayerBG2BGR, 3);
      cv::cvtColor(hig_raw8_, hig_rgb24_, COLOR_BayerBG2BGR, 3);
    } else if (type_ == ConvertType::kBayer2GB) {
      cv::cvtColor(low_raw8_, low_rgb24_, COLOR_BayerGB2BGR, 3);
      cv::cvtColor(mid_raw8_, mid_rgb24_, COLOR_BayerGB2BGR, 3);
      cv::cvtColor(hig_raw8_, hig_rgb24_, COLOR_BayerGB2BGR, 3);
    } else if (type_ == ConvertType::kBayer2RG) {
      cv::cvtColor(low_raw8_, low_rgb24_, COLOR_BayerRG2BGR, 3);
      cv::cvtColor(mid_raw8_, mid_rgb24_, COLOR_BayerRG2BGR, 3);
      cv::cvtColor(hig_raw8_, hig_rgb24_, COLOR_BayerRG2BGR, 3);
    } else if (type_ == ConvertType::kBayer2GR) {
      cv::cvtColor(low_raw8_, low_rgb24_, COLOR_BayerGR2BGR, 3);
      cv::cvtColor(mid_raw8_, mid_rgb24_, COLOR_BayerGR2BGR, 3);
      cv::cvtColor(hig_raw8_, hig_rgb24_, COLOR_BayerGR2BGR, 3);
    } else {
      PRINT(
          " %d current convert out of %d ~ %d", static_cast<int32_t>(type_),
          static_cast<int32_t>(ConvertType::kBayer2BG), static_cast<int32_t>(ConvertType::kBayer2GR));
      return;
    }
    for (int i = 0; i < height * width * 3; i++) {
      uint32_t tmp = (uint32_t)(*(low_rgb24_.data + i)) + ((uint32_t)(*(mid_rgb24_.data + i)) << 8) +
                     ((uint32_t)(*(hig_rgb24_.data + i)) << 16);
      *((float *)(hdr32_.data) + i) = tmp * 1.0 / 0xffffff;
    }
    tonemapReinhard_->process(hdr32_, ldrReinhard_);
    std::vector<uint8_t> tmp_v(width * height * 3, 0);
    for (int i = 0; i < height * width * 3; i++) {
      tmp_v[i] = *((float *)(ldrReinhard_.data) + i) * 255;
    }
    dst_img_ = cv::Mat(height, width, CV_8UC3, const_cast<uint8_t *>(tmp_v.data()), width * 3);
    imshow(win_name_, dst_img_);
    CreateTrackbar(win_name_);
    if (pic_tracbar_.trackbar_value != static_cast<int32_t>(SavePicMode::kDefault)) {
      SaveImage(dst_img_, memory, size_, save_pic_type_);
    }
    if (video_tracbar_.trackbar_value == static_cast<int32_t>(EnableMode::kON)) {
      if (new_video_) {
        video_tracbar_.pic_dir.clear();
        new_video_ = false;
      }
      SaveVideo(win_name_, width_, height_, video_fps, dst_img_);
    } else {
      new_video_ = true;
    }
    if (static_cast<int32_t>(save_type_) != save_pic_type_tracbar_.trackbar_value) {
      SetSavePicType(static_cast<SavePicType>(save_pic_type_tracbar_.trackbar_value));
    }
    int8_t key = static_cast<int8_t>(cv::waitKey(wait_ms_));
    if ((key == 's') || (key == 'S')) {
      pic_tracbar_.trackbar_value = static_cast<int32_t>(SavePicMode::kSingle);
      SaveImage(dst_img_, memory, size_, save_pic_type_);
    }
  }
}
ImageByte24::~ImageByte24() {
  if (init_) {
    cv::destroyWindow(win_name_);
  }
}
}  // namespace image

/**
 * @brief c interface for raw24 display
 *
 * unsigned char low_raw8[WIDTH * HEIGHT];
unsigned char mid_raw8[WIDTH * HEIGHT];
unsigned char high_raw8[WIDTH * HEIGHT];
unsigned int rgb24_buf_tt[WIDTH * HEIGHT * 3];
IplImage *raw_low8, *raw_mid8, *raw_high8;
IplImage *rgb_low8, *rgb_mid8, *rgb_high8;

  raw_low8 = cvCreateImage(cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 1);
  rgb_low8 = cvCreateImage(cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 3);

  raw_mid8 = cvCreateImage(cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 1);
  rgb_mid8 = cvCreateImage(cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 3);

  raw_high8 = cvCreateImage(cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 1);
  rgb_high8 = cvCreateImage(cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 3);

int8_t Display(void *data) {
  for (int i = 0; i < WIDTH * HEIGHT; i++) {
    low_raw8[i] = *((unsigned char *)(data) + i * 3);
    mid_raw8[i] = *((unsigned char *)(data) + i * 3 + 1);
    high_raw8[i] = *((unsigned char *)(data) + i * 3 + 2);
  }

  raw_low8->imageData = (char *)low_raw8;
  cvCvtColor(raw_low8, rgb_low8, COLOR_BayerBG2BGR);  // CV_BayerGB2BGR

  raw_mid8->imageData = (char *)mid_raw8;
  cvCvtColor(raw_mid8, rgb_mid8, COLOR_BayerBG2BGR);

  raw_high8->imageData = (char *)high_raw8;
  cvCvtColor(raw_high8, rgb_high8, COLOR_BayerBG2BGR);

  for (int i = 0; i < WIDTH * HEIGHT * 3; i++) {
    rgb24_buf_tt[i] = (unsigned int)(*(unsigned char *)(rgb_low8->imageData + i));
    rgb24_buf_tt[i] += (unsigned int)(*(unsigned char *)(rgb_mid8->imageData + i)) << 8;
    rgb24_buf_tt[i] += (unsigned int)(*(unsigned char *)(rgb_high8->imageData + i)) << 16;
  }

  Mat hdr32 = Mat(HEIGHT, WIDTH, CV_32FC3);
  float *p;
  for (int i = 0; i < HEIGHT; i++) {
    p = hdr32.ptr<float>(i);
    for (int j = 0; j < WIDTH * 3; j++) p[j] = rgb24_buf_tt[i * WIDTH * 3 + j] * 1.0 / 0xffffff;
  }

  tonemapReinhard->process(hdr32, ldrReinhard);
  ldrReinhard = 255 * ldrReinhard;

  Mat tmp = Mat(HEIGHT, WIDTH, CV_8UC3);
  uchar *q;
  for (int i = 0; i < HEIGHT; i++) {
    q = tmp.ptr<uchar>(i);
    p = ldrReinhard.ptr<float>(i);
    for (int j = 0; j < WIDTH * 3; j++) q[j] = (uchar)(*(p++));
  }
  Mat rsz_img = Mat::zeros(HEIGHT / RESIZE, WIDTH / RESIZE, CV_8UC3);
  resize(tmp, rsz_img, rsz_img.size());
  imshow("window_", rsz_img);
  int8_t key = static_cast<int8_t>(waitKey(5));
  return key;
}
 *
 */
