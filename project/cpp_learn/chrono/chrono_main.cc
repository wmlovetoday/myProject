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
  <ratio>  //用来实例化一个分数
  template <intmax_t N, intmax_t D = 1> class ratio;
  它有以下两个成员．分别是分子和分母
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

  std::cout << "1 kilogram has " << (std::kilo::num / std::kilo::den) << " grams";
  std::cout << std::endl;
}
/**
  <ratio>  //用来实例化一个分数
  template <class Rep, class Period = ratio<1> > class duration;
  它有以下两个成员．
  rep : count of period
  period :

 * Durations
   They measure time spans, like: one minute, two hours, or ten milliseconds. In this library,
   they are represented with objects of the duration class template,
   that couples a count representation and a period precision
   (e.g., ten milliseconds has ten as count representation and milliseconds as period precision).

   std::chrono::seconds(1);
   hours	signed integral type of at least 23 bits	ratio<3600,1>
   minutes	signed integral type of at least 29 bits	ratio<60,1>
   seconds	signed integral type of at least 35 bits	ratio<1,1>
   milliseconds	signed integral type of at least 45 bits	ratio<1,1000>
   microseconds	signed integral type of at least 55 bits	ratio<1,1000000>
   nanoseconds	signed integral type of at least 64 bits ratio<1,1000000000>
*/
static void DurationsTest() {
  base_con::PrintFlag("basic Durations test");

  std::chrono::duration<int, std::ratio<1, 1>> seconds_five{5};

  std::this_thread::sleep_for(std::chrono::seconds(1));
  PRINT("seconds_five.count = %d", seconds_five.count());
}
/**
 * Time points
 *   A reference to a specific point in time, like one's birthday, today's dawn,
 *   or when the next train passes. In this library, objects of the time_point
 *   class template express this by using a duration relative to an epoch (which
 *   is a fixed point in time common to all time_point objects using the same
 *   clock).

 * Clocks
 *   A framework that relates a time point to real physical time.
 *   The library provides at least three clocks that provide means to express
 *   the current time as a time_point: system_clock, steady_clock and
 *   high_resolution_clock.
*/

// which is a ratio type that expresses the number (or fraction) of seconds that
// elapse in each period.

// template <class Clock, class Duration = typename Clock::duration>  class
// time_point; A clock class, such as system_clock, steady_clock,
// high_resolution_clock or a custom clock class.
void BasicTimePointTest(void) {
  base_con::PrintFlag("basic TimePoint test");

  std::chrono::system_clock::time_point tp_epoch;  // epoch value
  std::chrono::time_point<std::chrono::system_clock, std::chrono::duration<int>> tp_seconds(
      std::chrono::duration<int>(1));
  std::chrono::system_clock::time_point tp(tp_seconds);
  std::cout << "1 second since system_clock epoch = ";
  std::cout << tp.time_since_epoch().count();
  std::cout << " system_clock periods." << std::endl;

  std::time_t tt = std::chrono::system_clock::to_time_t(tp);
  std::cout << "time_point tp is: " << ctime(&tt);
}

int main(int argc, char *argv[]) {
  try {
    RatioTest();
    DurationsTest();
    BasicTimePointTest();
  } catch (std::exception &err) {
    std::cout << "exception:" << err.what() << std::endl;
  } catch (...) {
    std::cout << "exception but do nothing" << std::endl;  // do nothing
    throw;                                                 //重新抛出
  }
  return 0;
}