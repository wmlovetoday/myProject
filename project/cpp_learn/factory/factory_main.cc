#include <cassert>  //assert
#include <condition_variable>
#include <cstddef>
#include <exception>  //static_assert
#include <iostream>
#include <iterator>
#include <memory>
#include <mutex>  // std::mutex, std::lock_guard
#include <string>

#include "base_context.h"
#include "com_interface.h"
#include "common_log.h"
#include "log_printf.h"

class Product {
 public:
  virtual ~Product() {}
  virtual std::string Operation() const = 0;
};

/**
 * Concrete Products provide various implementations of the Product interface.
 */
class ConcreteProduct1 : public Product {
 public:
  std::string Operation() const override { return "{Result of the ConcreteProduct1}"; }
};
class ConcreteProduct2 : public Product {
 public:
  std::string Operation() const override { return "{Result of the ConcreteProduct2}"; }
};

class Creator {
 public:
  virtual ~Creator(){};
  virtual Product *FactoryMethod() const = 0;

  std::string SomeOperation() const {
    // Call the factory method to create a Product object.
    Product *product = this->FactoryMethod();
    // Now, use the product.
    std::string result = "Creator: The same creator's code has just worked with " + product->Operation();
    delete product;
    return result;
  }
};
class ConcreteCreator1 : public Creator {
 public:
  Product *FactoryMethod() const override { return new ConcreteProduct1(); }
};

class ConcreteCreator2 : public Creator {
 public:
  Product *FactoryMethod() const override { return new ConcreteProduct2(); }
};

void ClientCode(const Creator &creator) {
  std::cout << "Client: I'm not aware of the creator's class, but it still works.\n"
            << creator.SomeOperation() << std::endl;
}

void FactoryTest() {
  base_con::PrintFlag("App: Launched with the ConcreteCreator1.");
  Creator *creator = new ConcreteCreator1();
  ClientCode(*creator);
  std::cout << std::endl;
  base_con::PrintFlag("App: Launched with the ConcreteCreator2.");
  Creator *creator2 = new ConcreteCreator2();
  ClientCode(*creator2);

  delete creator;
  delete creator2;
}

using ConPtr = std::unique_ptr<ConcreteProduct1>;
class Pointer {
 public:
  ConPtr GetConPtr() {
    ConcreteProduct1 con;
    std::unique_ptr<ConcreteProduct1> ptr = std::make_unique<ConcreteProduct1>(con);
    // return ptr;
    return std::move(ptr);
  }
};

template <typename T>
class tPointer {
 public:
  std::unique_ptr<T> GetConPtr() {
    T tmp;
    std::unique_ptr<T> ptr = std::make_unique<T>(tmp);
    return ptr;
  }
};

void PointerTest() {
  base_con::PrintFlag("pointer test");
  Pointer po;
  ConPtr ptr = po.GetConPtr();
  std::string st = ptr->Operation();
  PRINT("%s", st.data());
}

void TPointerTest() {
  base_con::PrintFlag("T pointer test");
  tPointer<ConcreteProduct1> po;
  auto ptr = po.GetConPtr();
  std::string st = ptr->Operation();
  PRINT("%s", st.data());
}

int main(int argc, char *argv[]) {
  try {
    FactoryTest();
    PointerTest();
    TPointerTest();
  } catch (std::exception &err) {
    std::cout << "exception:" << err.what() << std::endl;
  } catch (...) {
    std::cout << "exception but do nothing" << std::endl;  // do nothing
    throw;                                                 //重新抛出
  }
  return 0;
}
