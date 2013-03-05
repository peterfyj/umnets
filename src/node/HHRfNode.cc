#include "node/HHRfNode.h"
#include "driver/Driver.h"
#include "packet/packet.h"
#include "util/Math.h"
#include "network/network.h"
#include <utility>

HHRfNode* HHRfNode::create(Driver& driver) {
  return new HHRfNode(driver);
}

void HHRfNode::announce_options(Driver& driver) {
  driver.register_option("node.count", po::value<int>(), "count of all nodes");
  driver.register_option("node.f", po::value<int>(), "f for the 2HR-f");
}

HHRfNode::~HHRfNode() {
}

void HHRfNode::set_tag(int tag) {
  node_tag = tag;
}

int HHRfNode::get_tag() const {
  return node_tag;
}

const IntPos& HHRfNode::get_pos() const {
  return pos;
}

void HHRfNode::set_pos(IntPos& pos) {
  this->pos = pos;
}

void HHRfNode::set_dest(Node& node) {
  dest_node = &node;
}

void HHRfNode::add_packet(PacketPtr&& packet) {
  packet->set_src(*this);
  packet->set_dest(*dest_node);
  packet->set_tag(next_packet_tag++);
  packet->get_time_stamp().push_back(driver.get_tick());
  waiting_queue.push_back(std::move(packet));
}

HHRfNode& HHRfNode::get_dest() {
  return *dest_node;
}

void HHRfNode::scheduled() {
  Network& network = driver.get_network();
  auto iter_end = network.receiver_end(*this);
  for (auto iter = network.receiver_begin(*this); iter != iter_end; ++iter) {
    if (&iter->get_dest() == this) {
      SD(*iter);
      return;
    }
  }
  auto iter = Network::random_choose(network.receiver_begin(*this), iter_end);
  if (iter != iter_end) {
    if (Math::happen(1, 2)) {
      SR(*iter);
    } else {
      RD(*iter);
    }
  }
}

void HHRfNode::SD(HHRfNode& dest) {
  //TODO
}

void HHRfNode::SR(HHRfNode& relay) {
  //TODO
}

void HHRfNode::RD(HHRfNode& other_dest) {
  //TODO
}

HHRfNode::HHRfNode(Driver& driver) : driver(driver), next_packet_tag(0) {
}
