
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>

#include "base_context.h"
#include "common_log.h"
#include "log_printf.h"

using namespace std;

// todo

/**
  member constant	opening mode
  app	(append) Set the stream's position indicator to the end of the stream before each output operation.
  ate	(at end) Set the stream's position indicator to the end of the stream on opening.
  binary	(binary) Consider stream as binary rather than text.
  in	(input) Allow input operations on the stream.
  out	(output) Allow output operations on the stream.
  trunc	(truncate 截断) Any current content is discarded, assuming a length of zero on opening.
*/
/** Consructor
  explicit stringstream (ios_base::openmode which = ios_base::in | ios_base::out);
  explicit stringstream (const string& str,
                        ios_base::openmode which = ios_base::in | ios_base::out);
  stringstream (const stringstream&) = delete;
  stringstream (stringstream&& x);

  istream& operator>> (bool& val);
  istream& operator>> (short& val);
  istream& operator>> (unsigned short& val);
  istream& operator>> (int& val);
  istream& operator>> (unsigned int& val);
  istream& operator>> (long& val);
  istream& operator>> (unsigned long& val);
  istream& operator>> (long long& val);
  istream& operator>> (unsigned long long& val);
  istream& operator>> (float& val);
  istream& operator>> (double& val);
  istream& operator>> (long double& val);
  istream& operator>> (void*& val);
  stream buffers (2)
  istream& operator>> (streambuf* sb );
  manipulators (3)
  istream& operator>> (istream& (*pf)(istream&));
  istream& operator>> (ios& (*pf)(ios&));
  istream& operator>> (ios_base& (*pf)(ios_base&));

*/
static int32_t StreamConsructorTest() {
  base_con::PrintFlag("Stream Consructor Test");
  std::stringstream ss{};

  ss << 100 << ' ' << 200;  // must space interval
  int x, y;
  ss >> x >> y;
  PRINT("x = %d; y = %d", x, y);  // 利用此功能，可以实现字符串拆分
  PRINT("ss = %s", ss.str().data());

  std::stringstream tt{};
  tt << 100.0 << ' ' << 200.0;
  float i, j;
  tt >> i >> j;
  PRINT("i = %d; j = %d", i, j);  // 利用此功能，可以实现字符串拆分
}

static int32_t StreamRWTest() {
  base_con::PrintFlag("Stream read/write char format Test");
  std::fstream fs{"cpp.txt", std::ios_base::app | std::ios_base::trunc};
  if (!fs) {
    PRINT("open file failed");
    return -1;
  }
  if (fs.is_open()) {
    fs << "hello world";
  }
  PRINT("fs.is_open() = %d", fs.is_open());
  fs.close();
  return 0;
}
static int32_t StreamBinaryTest() {
  // https://www.shuzhiduo.com/A/1O5Eo17az7/

  base_con::PrintFlag("Stream read/write binary format Test");
  std::ofstream fs{"bin.txt", std::ios_base::binary | std::ios_base::trunc};
  if (!fs) {
    PRINT("open file failed");
    return -1;
  }
  if (fs.is_open()) {
    int nNum = 20;
    std::string str("Hello, world");
    fs.write((char *)&nNum, sizeof(int));
    fs.write(str.c_str(), sizeof(char) * (str.size()));
  }
  PRINT("fs.is_open() = %d", fs.is_open());
  fs.close();
  return 0;
}

static int32_t StringStreamTest() {
  printf("\n");
  PRINT("*************** %s ****************", "string stream test");
  printf("\n");
  std::ostringstream fs{"hello world", std::ios_base::ate};
  if (!fs) {
    PRINT("open file failed");
    return -1;
  }
  std::cout << fs.str() << '\n';
  PRINT("fs.str() = %s", fs.str().data());
  return 0;
}

static void OutputStringStream() {
  std::ostringstream fs{"V", std::ios_base::ate};
  char tmp[8];
  uint32_t ver = 0x50301;
  sprintf(tmp, "%d", static_cast<uint8_t>(ver));
  fs << tmp;
  sprintf(tmp, ".%d", static_cast<uint8_t>(ver >> 8));
  fs << tmp;
  sprintf(tmp, ".%d", static_cast<uint8_t>(ver >> 16));
  fs << tmp;
  PRINT("fs.str() = %s", fs.str().data());
}

int main(int argc, char *argv[]) {
  try {
    StreamConsructorTest();

  } catch (std::exception &err) {
    std::cout << "exception:" << err.what() << std::endl;
  } catch (...) {
    std::cout << "exception but do nothing" << std::endl;  // do nothing
    throw;                                                 //重新抛出
  }
  return 0;
}
