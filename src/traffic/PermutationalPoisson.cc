#include "traffic/PermutationalPoisson.h"
#include "driver/Driver.h"

void PermutationalPoisson::announce_options(Driver& driver) {
  driver.register_option("traffic.lambda", po::value<double>(),
      "average packet per slot");
}
