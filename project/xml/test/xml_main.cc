#include <cassert>  //assert
#include <cstdint>
#include <exception>  //static_assert
#include <iostream>
#include <string>

#include "base_context.h"
#include "common_log.h"
#include "log_printf.h"

#include "pugixml.hpp"

int XML_Test() {
  pugi::xml_document doc;
  pugi::xml_parse_result result = doc.load_file("xgconsole.xml");
  if (!result) return -1;

  for (pugi::xml_node tool : doc.child("Profile").child("Tools").children("Tool")) {
    int timeout = tool.attribute("Timeout").as_int();

    if (timeout > 0) std::cout << "Tool " << tool.attribute("Filename").value() << " has timeout " << timeout << "\n";
  }

  pugi::xpath_node_set tools_with_timeout = doc.select_nodes("/Profile/Tools/Tool[@Timeout > 0]");

  for (pugi::xpath_node node : tools_with_timeout) {
    pugi::xml_node tool = node.node();
    std::cout << "Tool " << tool.attribute("Filename").value() << " has timeout " << tool.attribute("Timeout").as_int()
              << "\n";
  }
}

int main(int argc, char *argv[]) {
  base_con::PrintFlag("opencv test");

  PTS_INFO("i am zlog !!!");
  PRINT("i am C log !!!");

  try {
    XML_Test();
  } catch (std::exception &err) {
    std::cout << "exception:" << err.what() << std::endl;
  } catch (...) {
    std::cout << "exception but do nothing" << std::endl;  // do nothing
    throw;                                                 //重新抛出
  }
  return 0;
}
