#include "traffic/PermutationalGeometric.h"
#include "driver/Driver.h"
#include "network/network.h"
#include "packet/packet.h"
#include "node/node.h"
#include "util/Math.h"

PermutationalGeometric* PermutationalGeometric::create(Driver& driver) {
  double lambda = driver.get_option<double>("traffic.lambda");
  bool one_link = driver.get_option<bool>("traffic.one_link_only");
  int tag = -1;
  if (one_link) {
    tag = driver.get_option<int>("traffic.one_link_sender");
  }
  return new PermutationalGeometric(driver, lambda, tag);
}

void PermutationalGeometric::announce_options(Driver& driver) {
  driver.register_option("traffic.lambda", po::value<double>(),
      "average packet per slot");
  driver.register_option("traffic.one_link_only", po::value<bool>(),
      "whether the traffic happens only in one link");
  driver.register_option("traffic.one_link_sender", po::value<int>(),
      "the node tag of the sender if traffic happens only in one link");
}

PermutationalGeometric::~PermutationalGeometric() {
}

void PermutationalGeometric::create_traffic() {
  using namespace std;
  Network& network = driver.get_network();
  int count = network.get_node_count();
  vector<Node*> all_node(count, nullptr);
  vector<int> derangement = Math::get_random_derangement(count);
  int i = 0;
  auto iter_end = network.end();
  for (auto iter = network.begin(); iter != iter_end; ++iter) {
    all_node[i++] = &*iter;
  }
  for (i = 0; i < count; ++i) {
    Node* d = all_node[derangement[i]];
    all_node[i]->set_dest(*d);
  }
}

void PermutationalGeometric::assign_packet(Node& node) {
  if (tagged_sender >= 0 && node.get_tag() != tagged_sender) {
    return;
  }
  if (Math::happen(lambda)) {
    node.add_packet(PacketPtr(Packet::create(driver)));
  }
}

PermutationalGeometric::PermutationalGeometric(Driver& driver, double lambda,
    int tag)
  : driver(driver)
  , lambda(lambda)
  , tagged_sender(tag) {
}
