#include "driver/Driver.h"
#include <iostream>

int main(int argc, char* argv[]) {
  Driver driver;
  driver.register_option("driver.loop", po::value<int>(), "loop times");
  if (!driver.init(argc, argv)) {
    return 1;
  }
  std::cout << driver.get_opt<int>("driver.loop") << std::endl;
}
