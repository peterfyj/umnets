#include "scheduler/TransGroup.h"
#include "driver/Driver.h"

void TransGroup::announce_options(Driver& driver) {
  driver.register_option("scheduler.alpha_x", po::value<int>(),
      "alpha for x scale of the transmission group algorithm");
  driver.register_option("scheduler.alpha_y", po::value<int>(),
      "alpha for y scale of the transmission group algorithm");
}
