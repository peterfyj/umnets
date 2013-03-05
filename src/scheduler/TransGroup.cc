#include "scheduler/TransGroup.h"
#include "network/network.h"
#include "node/node.h"
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

void TransGroup::schedule() {
  Network& network = driver.get_network();
  int size = network.get_size();
  for (int x = start_x; x < size; x += alpha_x) {
    for (int y = start_y; y < size; y += alpha_y) {
      auto iter_end = network.end(x, y);
      auto c = Network::random_choose(network.begin(x, y), iter_end);
      if (c != iter_end) {
        c->scheduled();
      }
    }
  }
  if (++start_x >= alpha_x) {
    start_x = 0;
    if (++start_y >= alpha_y) {
      start_y = 0;
    }
  }
}

TransGroup::TransGroup(Driver& driver, int alpha_x, int alpha_y)
  : driver(driver), alpha_x(alpha_x), alpha_y(alpha_y)
  , start_x(0), start_y(0) {
}
