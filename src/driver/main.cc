#include "driver/Driver.h"
#include "util/Debug.h"
#include <iostream>

Driver driver;

int main(int argc, char* argv[]) {
  driver.prepare_components();
  int code = driver.init(argc, argv);
  if (code >= 0) {
    return code;
  }
  driver.init_components();
  driver.start();
}
