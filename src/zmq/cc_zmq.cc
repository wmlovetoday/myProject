#include "cc_zmq.h"
#include <cstdint>

#define STR_FLAG "zmq : "
namespace zmq {

void Mq::RemoveMq() {
  if (nullptr != socket_) {
    int32_t ret = zmq_close(socket_);
    if (ret != 0) {
      PTS_ERROR("{} close mq socket failed 0x{:0x}:{}", STR_FLAG, zmq_errno(), zmq_strerror(errno));
    }
    socket_ = nullptr;
  }
  if (nullptr != context_) {
    int32_t ret = zmq_ctx_destroy(context_);
    if (ret != 0) {
      PTS_ERROR("{} destory mq contex failed 0x{:0x}:{}", STR_FLAG, zmq_errno(), zmq_strerror(errno));
    }
    context_ = nullptr;
  }
  init_ = false;
}

int32_t Mq::Init() {
  int32_t ret = dds::kCommuSuccess;
  std::string mq_ser_addr_{};
  if ((mq_type_ == dds::CommsType::kCommsSub) || (mq_type_ == dds::CommsType::kCommsReq)) {
    mq_ser_addr_.assign("tcp://");
    mq_ser_addr_ += addr_;
    mq_ser_addr_ += ":";
    mq_ser_addr_ += std::to_string(port_);
    ret = zmq_connect(socket_, mq_ser_addr_.data());
    PTS_INFO("{} connect mq {} {}", STR_FLAG, mq_ser_addr_.data(), ret);
    if (ret != 0) {
      PTS_ERROR("{} connect mq {} failed 0x{:0x}:{}", STR_FLAG, mq_ser_addr_.data(), zmq_errno(), zmq_strerror(errno));
      throw std::runtime_error{"connect mq failed"};
    }
    if (mq_type_ == dds::CommsType::kCommsSub) {
      ret = zmq_setsockopt(socket_, ZMQ_SUBSCRIBE, topic_.data(), topic_.size());
      if (ret != 0) {
        PTS_ERROR("{} set zmq sub topic failed 0x{:0x}:{}", STR_FLAG, zmq_errno(), zmq_strerror(errno));
      }
      PTS_INFO("{} set zmq sub topic {}", STR_FLAG, topic_);
    }
  } else if ((mq_type_ == dds::CommsType::kCommsPub) || (mq_type_ == dds::CommsType::kCommsRep)) {
    std::string tmp{};
    mq_ser_addr_.assign("tcp://");
    mq_ser_addr_ += addr_;
    mq_ser_addr_ += ":";
    mq_ser_addr_ += std::to_string(port_);
    // }
    ret = zmq_bind(socket_, mq_ser_addr_.c_str());
    PTS_INFO("{} mq bind {}", STR_FLAG, mq_ser_addr_.data());
    if (0 != ret) {
      PTS_ERROR("{} bind mq {} failed 0x{:0x}:{}", STR_FLAG, mq_ser_addr_.data(), zmq_errno(), zmq_strerror(errno));
      return dds::kCommuFailed;
    }
  } else {
    PTS_ERROR("{} unknown mq type {}", STR_FLAG, static_cast<int>(mq_type_));
    return dds::kCommuFailed;
  }
  return dds::kCommuSuccess;
}

Mq::Mq(dds::CommsType type, const std::string &addr, uint16_t port) : mq_type_(type), addr_(addr), port_(port) {
  int32_t ret = dds::kCommuSuccess;
  context_ = zmq_ctx_new();
  if (context_ == nullptr) {
    PTS_ERROR("{} creat mq context_ failed 0x{:0x}:{}", STR_FLAG, zmq_errno(), zmq_strerror(errno));
    RemoveMq();
    throw std::runtime_error{"creat mq context_ failed"};
  }
  socket_ = zmq_socket(context_, static_cast<int>(mq_type_));
  if (socket_ == nullptr) {
    PTS_ERROR("{} creat mq socket failed 0x{:0x}:{}", STR_FLAG, zmq_errno(), zmq_strerror(errno));
    RemoveMq();
    throw std::runtime_error{"creat mq socket failed"};
  }
  uint32_t value = LingerPeriodMs;
  ret = zmq_setsockopt(socket_, ZMQ_LINGER, &value, sizeof(value));
  if (ret != dds::kCommuSuccess) {
    RemoveMq();
    throw std::runtime_error{"set mq wait time failed"};
  }
  if (mq_type_ == dds::CommsType::kCommsReq) { /** add this attribute,if no ack, next
                                             send msg will success */
    uint32_t match_replies = 1;
    ret = zmq_setsockopt(socket_, ZMQ_REQ_RELAXED, &match_replies, sizeof(match_replies));
    if (ret != 0) {
      PTS_ERROR("{} set mq match replies failed 0x{:0x}:{}", STR_FLAG, zmq_errno(), zmq_strerror(errno));
      RemoveMq();
      throw std::runtime_error{"set mq match replies failed"};
    }
    uint32_t dis_check = 1;
    ret = zmq_setsockopt(socket_, ZMQ_REQ_CORRELATE, &dis_check, sizeof(dis_check));
    if (ret != 0) {
      PTS_ERROR("{} set mq do not check rep failed 0x{:0x}:{}", STR_FLAG, zmq_errno(), zmq_strerror(errno));
      RemoveMq();
      throw std::runtime_error{"set mq do not check rep failed"};
    }
  }
  ret = Init();
  if (ret != dds::kCommuSuccess) {
    RemoveMq();
    throw std::runtime_error{"init mq type failed"};
  }
  PTS_INFO("{} creat mq success", STR_FLAG);
  init_ = true;
}

int32_t Mq::Send(const void *buf, size_t len, int flags) {
  int32_t ret = 0;
  if (!init_) {
    PTS_ERROR("{} mq not init", STR_FLAG);
    return dds::kCommuFailed;
  }
  ret = zmq_send(socket_, buf, len, flags);
  if (static_cast<size_t>(ret) >= len) {
    return ret;
  }
  PTS_ERROR("{} msg send failed 0x{:0x}:{}", STR_FLAG, zmq_errno(), zmq_strerror(errno));
  return dds::kCommuFailed;
}

int32_t Mq::Write(const void *buf, uint32_t len, const std::string &topic) {
  if (buf == nullptr) {
    PTS_ERROR("{} arguments illegal", STR_FLAG);
    return dds::kCommuFailed;
  }
  if (!init_) {
    PTS_ERROR("{} mq not init", STR_FLAG);
    return dds::kCommuFailed;
  }
  if (topic.empty()) {
    int32_t ret = Send(buf, len, 0);
    return ret;
  }
  int32_t ret = Send(topic.data(), topic.size(), ZMQ_SNDMORE);
  if (ret < 0) {
    return dds::kCommuFailed;
  }
  ret = Send(buf, len, 0);
  if (ret < 0) {
    return dds::kCommuFailed;
  }
  return ret;
}

int32_t Mq::Receive(zmq_msg_t &part, uint32_t wait_ms, const std::string &topic) {
  if (!init_) {
    PTS_ERROR("{} mq not init", STR_FLAG);
    return dds::kCommuFailed;
  }

  int32_t ret = 0;
  zmq_pollitem_t items[] = {{socket_, 0, ZMQ_POLLIN, 0}};
  ret = zmq_poll(items, 1, wait_ms);
  if (ret >= 0) {
    if (items[0].revents & ZMQ_POLLIN) {
      ret = zmq_msg_init(&part);
      if (0 == ret) {
        ret = zmq_msg_recv(&part, socket_, 0);
        if (ret >= 0) {
          if (topic.empty()) {
            return (ret); /** free by call function*/
          }

          bool compared = true;
          if (static_cast<size_t>(ret) != topic.size()) {
            compared = false;
          }
          if (compared) {
            auto *ack = reinterpret_cast<char *>(zmq_msg_data(&part));
            for (size_t i = 0; i < topic.size(); i++) {
              if (topic[i] != *(ack + i)) {
                PTS_ERROR("{} topic compare failed", STR_FLAG);
                compared = false;
                break;
              }
            }
          }
          if (compared) {
            zmq_msg_close(&part); /** free topic*/
            int32_t more = 0;
            size_t more_size = sizeof(more);
            ret = zmq_getsockopt(socket_, ZMQ_RCVMORE, &more, &more_size);
            if ((0 == ret) && (more > 0)) {
              ret = zmq_msg_init(&part); /** new message*/
              if (0 == ret) {
                ret = zmq_msg_recv(&part, socket_, 0);
                if (ret >= 0) {
                  return (ret); /** free by call function*/
                }
                zmq_msg_close(&part); /** free message*/
                PTS_ERROR("{} msg recv failed 0x{:0x}:{}", STR_FLAG, zmq_errno(), zmq_strerror(errno));
              }
              PTS_ERROR("{} msg init failed 0x{:0x}:{}", STR_FLAG, zmq_errno(), zmq_strerror(errno));
            }
            PTS_ERROR("{} get more failed 0x{:0x}:{}", STR_FLAG, zmq_errno(), zmq_strerror(errno));
          }
          zmq_msg_close(&part); /** free topic*/
          PTS_ERROR("{} topic compare failed 0x{:0x}:{}", STR_FLAG, zmq_errno(), zmq_strerror(errno));
        }
        PTS_ERROR("{} msg recv failed 0x{:0x}:{}", STR_FLAG, zmq_errno(), zmq_strerror(errno));
      }
      PTS_ERROR("{} msg init failed 0x{:0x}:{}", STR_FLAG, zmq_errno(), zmq_strerror(errno));
    }
    PTS_ERROR("{} msg recv nothing 0x{:0x}:{}", STR_FLAG, zmq_errno(), zmq_strerror(errno));
  }
  PTS_ERROR("{} msg pull failed 0x{:0x}:{}", STR_FLAG, zmq_errno(), zmq_strerror(errno));
  return dds::kCommuFailed;
}
int32_t Mq::Read(void *buf, uint32_t len, uint32_t wait_ms, const std::string &topic) {
  zmq_msg_t msg{};
  int32_t cpy_len = 0;
  int32_t ret = Receive(msg, wait_ms, topic);
  if (ret > 0) {
    if (static_cast<uint32_t>(ret) <= len) {
      cpy_len = ret;
    } else {
      cpy_len = len;
    }
    auto *ack = reinterpret_cast<uint8_t *>(zmq_msg_data(&msg));
    // std::string info{};
    // info.insert(info.begin(), ack, ack + ret); // zmq_msg_size(&msg)
    memcpy(buf, ack, static_cast<size_t>(cpy_len));
    zmq_msg_close(&msg);
    return cpy_len;
  }
  return ret;
}
}  // namespace zmq