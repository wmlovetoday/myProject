#include <cstdint>
#include <stdexcept>

#include "sys_terminal.h"

namespace sys {
int32_t Terminal::Open(const std::string& serial) {
  ter_name = serial;
  fd = open(ter_name.c_str(), O_RDWR | O_NOCTTY);  //| O_NDELAY
  if (fd == -1) {
    PRINT_ERRNO("open %s failed", ter_name.c_str());
    return -1;
  }
  // 设置阻塞模式
  int ret = fcntl(fd, F_GETFL, 0);
  termios tmp_attrs{};
  ret = tcgetattr(fd, &tmp_attrs);
  if (ret != 0) {
    PRINT_ERRNO("get %s attribute failed", ter_name.c_str());
  }
  // Enable the receiver and set local mode.
  ter_attrs.c_cflag |= CLOCAL | CREAD;

  // Setting the Baud Rate
  ret = cfsetispeed(&ter_attrs, B115200);
  if (ret != 0) {
    PRINT_ERRNO("set %s in speed failed", ter_name.c_str());
  }
  ret = cfsetospeed(&ter_attrs, B115200);
  if (ret != 0) {
    PRINT_ERRNO("set %s out speed failed", ter_name.c_str());
  }
  // 设置数据位 8n1
  // 设置奇偶校验位(no parity)
  ter_attrs.c_cflag &= ~PARENB;
  // 设置停止位
  ter_attrs.c_cflag &= ~CSTOPB;
  ter_attrs.c_cflag &= ~CSIZE;
  ter_attrs.c_cflag |= CS8;

  // no Hardware Flow Control
  // ter_attrs.c_cflag &= ~CNEW_RTSCTS;
  ter_attrs.c_cc[VTIME] = 0;
  ter_attrs.c_cc[VMIN] = 9;
  ret = tcflush(fd, TCIFLUSH);  // 清空接收缓冲区的数据
  if (ret != 0) {
    PRINT_ERRNO("flush %s recv buffer failed", ter_name.c_str());
  }
  ret = tcflush(fd, TCOFLUSH);  // 清空发送缓冲区的数据
  if (ret != 0) {
    PRINT_ERRNO("flush %s send buffer failed", ter_name.c_str());
  }
  ret = tcsetattr(fd, TCSANOW, &ter_attrs);
  if (ret < 0) {
    PRINT_ERRNO("set %s attribute failed", ter_name.c_str());
  }
  PRINT("open %s success", ter_name.c_str());
  return 0;
}

int32_t Terminal::Send(const uint8_t* buffer, uint32_t size) {
  ssize_t s_len = write(fd, buffer, size);
  if (s_len == size) {
    return (int32_t)(s_len);
  } else {
    PRINT_ERRNO("%s send failed", ter_name.c_str());
    tcflush(fd, TCOFLUSH);
    return -1;
  }
}

int32_t Terminal::Recv(uint8_t* buffer, uint32_t size) {
  int32_t ret = 0;
  while (size) {
    ssize_t len = read(fd, buffer + ret, 1);
    if (len == 1) {
      ret++;
      size--;
    }
    if (len != 1) {
      PRINT_ERRNO("%s read failed", ter_name.c_str());
      return -1;
    }
  }
  return ret;
}

int32_t Terminal::RecvByTerminator(char terminator, uint8_t* buffer, uint32_t size) {
  int32_t ret = 0;
  while (size) {
    ssize_t len = read(fd, buffer + ret, 1);
    if (len == 1) {
      if (*(buffer + ret) == terminator) {
        ret++;
        break;
      }
      ret++;
      size--;
    }
    if (len != 1) {
      PRINT_ERRNO("%s read failed", ter_name.c_str());
      return -1;
    }
  }
  return ret;
}

int32_t Terminal::SelfTest() {
  uint32_t sec = 5;
  int32_t ret = 0;
  uint8_t send = '1';
  uint8_t recv = '0';
  while (sec) {
    send += 1;
    ret = Send(&send, 1);
    if (ret == 1) {
      ret = Recv(&recv, 1);
      if (ret == 1) {
        PRINT("send : %c; recv %c", send, recv);
      }
    }
    sleep(1);
    sec--;
  }
  return 0;
}

}  // namespace sys
