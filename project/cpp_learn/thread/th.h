#ifndef CPP_LEARN_THREAD_TH
#define CPP_LEARN_THREAD_TH

#include <cstdint>
#include <functional>
#include <thread>

#include "log_printf.h"

// std::function<返回值(参数列表)>变量{函数体}．

using Callback = std::function<int32_t(uint8_t *data, uint32_t size)>;
class Th {
public:
  Th() = default;
  int32_t Regist(Callback const &call_back) noexcept;
  int32_t Remove() noexcept;
  /** start call back func*/
  inline void StartCallback() noexcept { start_run_ = true; }
  inline void StopCallback() noexcept { start_run_ = false; }
  ~Th();

private:
  bool start_run_{false};
  bool init_{false};
  bool running_{false};
  uint8_t test_ = 10;
  std::thread acquire_{};
  Callback call_back_inc_{};
  void AcquireThread(uint8_t test);
};
#endif

/*
https://cplusplus.com/reference/functional/bind/
void BindTest(void)
{
    PRINT("*************** %s ****************", "basic bind test");
    PRINT("*************** static void Func(int a, int b, int c)
****************"); PRINT("call : auto f = std::bind(Func,4,5,6);"); auto f =
std::bind(Func,4,5,6); f();

    PRINT("call : auto f = std::bind(Func,std::placeholders::_1, 5, 6);");
    auto f1 = std::bind(Func, std::placeholders::_1, 5, 6);
        f1(100);

    PRINT("call : auto f = std::bind(Func,4, std::placeholders::_2, 6);");
        auto f2 = std::bind(Func, 4, std::placeholders::_2, 6);
        f2(0, 100);

    PRINT("call : auto f = std::bind(Func, 4, 5, std::placeholders::_3);");
        auto f3 = std::bind(Func, 4, 5, std::placeholders::_3);
        f3(0, 0, 100);

    //使用 function类也可以保存 bind绑定的函数,
        //但是要注意参数数量和 bind绑定的参数数量
    PRINT("call : td::function<void()> p = std::bind(Func, 7, 8, 9);");
        std::function<void()> p = std::bind(Func, 7, 8, 9);
        p();

    PRINT("call : std::function<void(int, int)> p1 = std::bind(Func, 7,
std::placeholders::_2, 9);"); std::function<void(int, int)> p1 = std::bind(Func,
7, std::placeholders::_2, 9); p1(0, 100);

        //使用 bind函数改变函数的参数顺序
    PRINT("call : auto f4 = std::bind(Func, std::placeholders::_3,
std::placeholders::_2, std::placeholders::_1);"); auto f4 = std::bind(Func,
std::placeholders::_3, std::placeholders::_2, std::placeholders::_1); f4(1, 2,
3);
}
*/