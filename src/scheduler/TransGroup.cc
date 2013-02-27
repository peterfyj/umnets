#include "scheduler/TransGroup.h"
#include "driver/Driver.h"

TransGroup* TransGroup::create(Driver& driver) {
  int ax = driver.get_option<int>("scheduler.alpha_x");
  int ay = driver.get_option<int>("scheduler.alpha_y");
  return new TransGroup(driver, ax, ay);
}

void TransGroup::announce_options(Driver& driver) {
  driver.register_option("scheduler.alpha_x", po::value<int>(),
      "alpha for x scale of the transmission group algorithm");
  driver.register_option("scheduler.alpha_y", po::value<int>(),
      "alpha for y scale of the transmission group algorithm");
}

TransGroup::TransGroup(Driver& driver, int alpha_x, int alpha_y)
  : driver(driver), alpha_x(alpha_x), alpha_y(alpha_y) {
}
