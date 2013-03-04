#include "motion/IID.h"
#include "driver/Driver.h"
#include "util/Math.h"
#include "network/network.h"
#include "node/node.h"

IID* IID::create(Driver& driver) {
  return new IID(driver, driver.get_option<int>("motion.step"));
}

void IID::announce_options(Driver& driver) {
  driver.register_option("motion.step", po::value<int>(),
      "time slots between two moving steps");
}

IntPos IID::random_place(Node& node) const {
  int size = driver.get_network().get_size();
  int x = Math::get_random_int(0, size - 1);
  int y = Math::get_random_int(0, size - 1);
  return IntPos(x, y);
}

IntPos IID::random_move(Node& node) {
  if (driver.get_tick() % step != 0) {
    return node.get_pos();
  } else {
    return random_place(node);
  }
}

IID::IID(Driver& driver, int step)
  : driver(driver), step(step) {
}
