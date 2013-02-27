#include "node/HHRfNode.h"
#include "driver/Driver.h"

HHRfNode* HHRfNode::create(Driver& driver, int tag) {
  return new HHRfNode(tag);
}

void HHRfNode::announce_options(Driver& driver) {
  driver.register_option("node.count", po::value<int>(), "count of all nodes");
  driver.register_option("node.f", po::value<int>(), "f for the 2HR-f");
}

HHRfNode::~HHRfNode() {
}

int HHRfNode::get_tag() const {
  return my_tag;
}

const IntPos& HHRfNode::get_pos() const {
  return pos;
}

void HHRfNode::set_pos(IntPos& pos) {
  this->pos = pos;
}

HHRfNode::HHRfNode(int tag) : my_tag(tag) {
}
