#include "driver/Driver.h"
#include <iostream>

int main(int argc, char* argv[]) {
  Driver driver;
  driver.prepare_components();
  if (!driver.init(argc, argv)) {
    return 1;
  }
  driver.init_components();
}
