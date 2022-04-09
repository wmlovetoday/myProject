#include <cassert>  //assert
#include <condition_variable>
#include <exception>  //static_assert
#include <iostream>
#include <iterator>
#include <mutex>  // std::mutex, std::lock_guard
#include <string>

#include "base_context.h"
#include "common_log.h"
#include "log_printf.h"

// :RAII: Resource Acquisition Is Initialization 资源获取即初始化,自动构造与释放

// https://blog.csdn.net/m0_46657980/article/details/109385050
// 常规手段没办法访问类的私有成员，折衷，于是就有了 friend 关键字。
/**
 * 友元分为两种 : 友元函数和友元类
 * 友元函数：
 * 　　全局函数，直接访问类的私有成员。
 *　　　类的私有成员，这样可以直接使用类对象调用私有成员函数
 * 　　其它类的成员函数，可以使用其它类对象访问另一个类的私有成员变量
 *
 * 友元类：
 * 直接在Ｂ中声明Ａ是自己的友元类，那么Ａ就可以直接访问Ｂ的所有元素。
 */
class CCar;

class CDriver {
 public:
  void ModifyCar(CCar *pCar);  //改装汽车
};
class CCar {
 public:
  void SetPrice(int p) { price = p; }

 private:
  int price;
  friend int MostExpensiveCar(CCar cars[], int total);  //声明友元
  friend void CDriver::ModifyCar(CCar *pCar);           //声明友元
  friend void UpdateCar(CCar *pCar);
};
void CDriver::ModifyCar(CCar *pCar) {
  pCar->price += 50;  //汽车改装后价值增加
}
int MostExpensiveCar(CCar cars[], int total)  //求最贵气车的价格
{
  int tmpMax = -1;
  for (int i = 0; i < total; ++i)
    if (cars[i].price > tmpMax) tmpMax = cars[i].price;
  return tmpMax;
}
void UpdateCar(CCar *pCar) {
  pCar->price *= 10;  //汽车升级后价值翻十倍
}
void FriendFunc() {
  base_con::PrintFlag("friend function test");
  CCar car[3]{};
  car[0].SetPrice(10);
  car[1].SetPrice(20);
  car[2].SetPrice(30);
  int max = MostExpensiveCar(car, 3);
  PRINT("max price %d", max);
  CDriver dr;
  dr.ModifyCar(&car[1]);
  max = MostExpensiveCar(car, 3);
  PRINT("after modify, max price %d", max);
  UpdateCar(&car[2]);
  max = MostExpensiveCar(car, 3);
  PRINT("after update, max price %d", max);
}

class XiaoMing {
 public:
  int GetSore() { return score; }

 private:
  std::string name;
  int old;
  int score;
  friend class Teacher;
};
class Teacher {
 public:
  void SetAverageScore(XiaoMing &who, int score);
};
void Teacher::SetAverageScore(XiaoMing &who, int count) { who.score = count; }

void FriendClass() {
  base_con::PrintFlag("friend class test");
  Teacher te;
  XiaoMing stu;
  te.SetAverageScore(stu, 60);
  PRINT("XiaoMing's score %d", stu.GetSore());
}

int main(int argc, char *argv[]) {
  try {
    FriendFunc();
    FriendClass();
  } catch (std::exception &err) {
    std::cout << "exception:" << err.what() << std::endl;
  } catch (...) {
    std::cout << "exception but do nothing" << std::endl;  // do nothing
    throw;                                                 //重新抛出
  }
  return 0;
}
