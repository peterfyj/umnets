#include "traffic/PermutationalPoisson.h"
#include "driver/Driver.h"
#include "network/network.h"
#include "node/node.h"
#include "util/Math.h"

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

void PermutationalPoisson::create_traffic() {
  using namespace std;
  Network& network = driver.get_network();
  int count = network.get_node_count();
  vector<Node*> all_node(count, nullptr);
  vector<int> derangement = Math::get_random_derangement(count);
  int i = 0;
  for (auto iter = network.begin(); iter != network.end(); ++iter) {
    all_node[i++] = iter.operator->();
  }
  for (i = 0; i < count; ++i) {
    Node* d = all_node[derangement[i]];
    all_node[i]->set_dest_node(*d);
  }
}

PermutationalPoisson::PermutationalPoisson(Driver& driver, double lambda)
  : driver(driver), lambda(lambda) {
}
