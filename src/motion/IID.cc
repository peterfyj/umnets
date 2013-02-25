#include "motion/IID.h"
#include "driver/Driver.h"

void IID::announce_options(Driver& driver) {
  driver.register_option("motion.step", po::value<int>(),
      "time slots between two moving steps");
}
