#include "traffic/PermutationalPoisson.h"
#include "driver/Driver.h"

PermutationalPoisson* PermutationalPoisson::create(Driver& driver) {
  double lambda = driver.get_option<double>("traffic.lambda");
  return new PermutationalPoisson(driver, lambda);
}

void PermutationalPoisson::announce_options(Driver& driver) {
  driver.register_option("traffic.lambda", po::value<double>(),
      "average packet per slot");
}

PermutationalPoisson::~PermutationalPoisson() {
}

PermutationalPoisson::PermutationalPoisson(Driver& driver, double lambda)
  : driver(driver), lambda(lambda) {
}
