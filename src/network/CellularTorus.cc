#include "network/CellularTorus.h"
#include "driver/Driver.h"

void CellularTorus::announce_options(Driver& driver) {
  driver.register_option("network.size", po::value<int>(),
      "cell count for both scales");
}
