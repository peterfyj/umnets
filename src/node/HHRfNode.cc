#include "node/HHRfNode.h"
#include "driver/Driver.h"
#include "packet/packet.h"
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

HHRfNode::HHRfNode(Driver& driver) : driver(driver), next_packet_tag(0) {
}
