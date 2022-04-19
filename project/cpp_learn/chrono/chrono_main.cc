#include <cassert>  //assert
#include <chrono>
#include <exception>  //static_assert
#include <iostream>
#include <iterator>
#include <ratio>
#include <string>
#include <thread>

#include "base_context.h"
#include "common_log.h"
#include "log_printf.h"
/**
 * @brief
 * chrono 既是一个头文件，也是一个命名空间
 *
 * Durations / Time points / Clocks
 *
 * template <class Rep, class Period = ratio<1> > class std::chrono::duration;
 *
 * 用来表达一段时间。Rep 用来表示计数类型
 * count() 成员函数;
 *
// hours	      signed integral type of at least 23 bits	ratio<3600,1>
// minutes	    signed integral type of at least 29 bits	ratio<60,1>
// seconds	    signed integral type of at least 35 bits	ratio<1,1>
// milliseconds	signed integral type of at least 45 bits	ratio<1,1000>
// microseconds	signed integral type of at least 55 bits	ratio<1,1000000>
// nanoseconds	signed integral type of at least 64 bits  ratio<1,1000000000>
 * 使用方法:  std::chrono::seconds

 * static member functions:
 * static constexpr duration zero() {
 *   return duration_values<rep>::zero();
 *   }
 */
using seconds_type = std::chrono::duration<int>;
using milliseconds_type = std::chrono::duration<int, std::milli>;
using hours_type = std::chrono::duration<int, std::ratio<60 * 60>>;

static void DurationsTest() {
  base_con::PrintFlag("basic Durations test");

  std::chrono::duration<int, std::ratio<1, 1>> seconds_five{5};
  PRINT("seconds_five.count = %d", seconds_five.count());

  std::chrono::seconds sec(2);
  PRINT("std::chrono::seconds(2).count() = %ld", sec.count());

  std::chrono::steady_clock::duration zd = std::chrono::steady_clock::duration::zero();
  std::cout << "std::chrono::zero() = " << zd.count() << std::endl;

  seconds_type s{2};
  milliseconds_type ms{2};
  hours_type h{2};

  PRINT("2 sec.count = %d", s.count());
  PRINT("2 ms.count = %d", ms.count());
  PRINT("2 hour.count = %d", h.count());

  std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
  std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();

  std::chrono::steady_clock::duration d = t2 - t1;

  if (d == std::chrono::steady_clock::duration::zero())
    std::cout << "The internal clock did not tick.\n";
  else
    std::cout << "The internal clock advanced " << d.count() << " periods.\n";
}

/**
  <ratio>
  template <intmax_t N, intmax_t D = 1> class ratio;
  //用来实例化一个分数,它有以下两个成员变量．分别是分子和分母
  num	: Numerator
  den	: Denominator

  已经定义好了一些 ratio
  nano  : ratio<1,1000000000>
  micro	ratio<1,1000000>	10^-6
  milli	ratio<1,1000>	10^-3
  centi	ratio<1,100>	10^-2
  deci	ratio<1,10>	10^-1
  deca	ratio<10,1>	10^1
  hecto	ratio<100,1>	10^2
  kilo	ratio<1000,1>	10^3
  mega	ratio<1000000,1>
*/

static void RatioTest() {
  base_con::PrintFlag("basic Ratio test");
  std::ratio<1, 3> one_third;
  std::ratio<2, 4> two_fourths;

  std::cout << "one_third= " << one_third.num << "/" << one_third.den << std::endl;
  std::cout << "two_fourths= " << two_fourths.num << "/" << two_fourths.den << std::endl;

  using one_fourths = std::ratio<1, 4>;
  using one_thirds = std::ratio<1, 3>;
  using sum = std::ratio_add<one_fourths, one_thirds>;

  std::cout << "sum= " << sum::num << "/" << sum::den;
  std::cout << " (which is: " << (double(sum::num) / sum::den) << ")" << std::endl;

  std::cout << "1 kilogram has " << (std::kilo::num / std::kilo::den) << " grams" << std::endl;
}

/**
 template <class ToDuration, class Rep, class Period>
constexpr ToDuration duration_cast (const duration<Rep,Period>& dtn);
 */

static void DurationsCastTest() {
  base_con::PrintFlag("basic Durations cast test");

  std::chrono::duration<int> seconds_three{3};
  auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(seconds_three);
  PRINT("ms = %ld; ms.count() = %ld", ms, ms.count());

  // std::this_thread::sleep_for(std::chrono::seconds(1));
}
/**
 * Time points
  std::chrono::time_point
  template <class Clock, class Duration = typename Clock::duration> class time_point;
  成员变量:
    clock;duration;rep;period;
  成员函数：
    duration time_since_epoch

构造函数：
time_point(); //1
template <class Duration2>
time_point (const time_point<clock,Duration2>& tp);//2
explicit time_point (const duration& dtn); //3
*/

void BasicTimePointTest(void) {
  base_con::PrintFlag("basic TimePoint test");

  std::chrono::system_clock::time_point tp_epoch;  // epoch value
  std::chrono::time_point<std::chrono::system_clock, std::chrono::duration<int>> tp_seconds(
      std::chrono::duration<int>(1));

  std::chrono::system_clock::time_point tp(tp_seconds);

  std::cout << "1 second since system_clock epoch = " << tp.time_since_epoch().count() << " system_clock periods."
            << std::endl;

  std::time_t tt = std::chrono::system_clock::to_time_t(tp);
  std::cout << "time_point tp is: " << ctime(&tt);
}

/*
std::chrono::system_clock
to_time_t
  成员变量:
    time_point;duration;rep;period;
  成员函数：
    is_steady
  静态成员函数
  static time_point now()         //Get current time (public static member function )
  time_t to_time_t(const time_point& tp)     //Convert to time_t ( public static member function )
  from_time_t   //Convert from time_t ( public static member function )

*/

void ClockTest() {
  base_con::PrintFlag("clock test");

  using std::chrono::system_clock;
  std::chrono::duration<int, std::ratio<60 * 60 * 24>> one_day(1);

  system_clock::time_point today = system_clock::now();
  system_clock::time_point tomorrow = today + one_day;

  std::time_t tt;

  tt = system_clock::to_time_t(today);
  std::cout << "today is: " << ctime(&tt);

  tt = system_clock::to_time_t(tomorrow);
  std::cout << "tomorrow will be: " << ctime(&tt);
}
/*
 template <class ToDuration, class Rep, class Period>
constexpr ToDuration duration_cast (const duration<Rep,Period>& dtn);

template<class ToDuration, class Clock, class Duration> time_point<Clock, ToDuration> time_point_cast(
    const time_point<Clock, Duration> &tp);
*/

void BasicMsCounter(void) {
  base_con::PrintFlag("basic counter test");

  auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch())
                .count();
  auto sec =
      std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
  std::cout << "sec : " << sec << " ms " << ms << std::endl;
}

int main(int argc, char *argv[]) {
  try {
    RatioTest();
    DurationsTest();
    DurationsCastTest();
    BasicTimePointTest();
    BasicMsCounter();
  } catch (std::exception &err) {
    std::cout << "exception:" << err.what() << std::endl;
  } catch (...) {
    std::cout << "exception but do nothing" << std::endl;  // do nothing
    throw;                                                 //重新抛出
  }
  return 0;
}