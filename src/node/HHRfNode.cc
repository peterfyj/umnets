#include "node/HHRfNode.h"
#include "driver/Driver.h"
#include "packet/packet.h"
#include "util/Math.h"
#include "network/network.h"
#include "logger/logger.h"
#include <utility>
#include <iterator>

HHRfNode* HHRfNode::create(Driver& driver) {
  return new HHRfNode(driver, driver.get_option<int>("node.f"));
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
  driver.get_logger().node_moved(*this);
  this->pos = pos;
}

void HHRfNode::set_dest(Node& node) {
  dest_node = &node;
}

void HHRfNode::add_packet(PacketPtr&& packet) {
  packet->set_src(*this);
  packet->set_dest(*dest_node);
  packet->set_tag(request_map[dest_node]++);
  packet->get_time_stamp().push_back(driver.get_tick());
  if (waiting_queue.empty()) {
    packet->get_time_stamp().push_back(driver.get_tick());
  }
  driver.get_logger().packet_generated(*this, *packet);
  waiting_queue.push_back(std::move(packet));
}

HHRfNode& HHRfNode::get_dest() {
  return *dest_node;
}

void HHRfNode::scheduled() {
  Network& network = driver.get_network();
  auto iter_end = network.receiver_end(*this);
  for (auto iter = network.receiver_begin(*this); iter != iter_end; ++iter) {
    if (dest_node == &*iter) {
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

void HHRfNode::receive(HHRfNode& src, PacketPtr&& packet) {
  packet->get_time_stamp().push_back(driver.get_tick());
  driver.get_logger().packet_transfered(src, *this, *packet);
  if (&packet->get_dest() == this) {  // SD or RD.
    ++request_map[this];
    packet = nullptr;
  } else { // SR.
    request_map[&packet->get_dest()] = packet->get_tag() + 1;
    relay_map[&packet->get_dest()].push_back(std::move(packet));
  }
}

void HHRfNode::SD(HHRfNode& dest) {
  int tag = dest.request_map[&dest];
  auto iter = find_in_sequent_queue(sent_queue, tag);
  if (iter != sent_queue.end()) {
    dest.receive(*this, std::move(*iter));
    sent_queue.erase(sent_queue.begin(), ++iter);
    return;
  }
  iter = find_in_sequent_queue(waiting_queue, tag);
  if (iter != waiting_queue.end()) {
    if (iter != waiting_queue.begin()) {
      (*iter)->get_time_stamp().push_back(driver.get_tick());
    }
    dest.receive(*this, std::move(*iter));
    dispatched = 0;
    waiting_queue.erase(waiting_queue.begin(), ++iter);
    if (!waiting_queue.empty()) {
      waiting_queue.front()->get_time_stamp().push_back(driver.get_tick());
    }
    sent_queue.clear();
  }
}

void HHRfNode::SR(HHRfNode& relay) {
  if (waiting_queue.empty()) {
    return;
  }
  PacketPtr& ptr = waiting_queue.front();
  if (relay.request_map[dest_node] > ptr->get_tag()) {
    return;
  }
  relay.receive(*this, PacketPtr(ptr->clone()));
  if (++dispatched >= f) {
    dispatched = 0;
    sent_queue.splice(sent_queue.end(), waiting_queue, waiting_queue.begin());
    if (!waiting_queue.empty()) {
      waiting_queue.front()->get_time_stamp().push_back(driver.get_tick());
    }
  }
}

void HHRfNode::RD(HHRfNode& other_dest) {
  Queue& relay_queue = relay_map[&other_dest];
  int tag = other_dest.request_map[&other_dest];
  while (!relay_queue.empty()) {
    int qtag = relay_queue.front()->get_tag();
    if (qtag < tag) {
      relay_queue.pop_front();
    } else {
      if (qtag == tag) {
        other_dest.receive(*this, std::move(relay_queue.front()));
        relay_queue.pop_front();
      }
      break;
    }
  }
}

auto HHRfNode::find_in_sequent_queue(Queue& q, int tag) -> Queue::iterator {
  if (q.empty()) {
    return q.end();
  }
  int front_tag = q.front()->get_tag();
  int back_tag = q.back()->get_tag();
  if (front_tag > tag || back_tag < tag) {
    return q.end();
  }
  if (back_tag + front_tag > (tag << 1)) {  // Closer to front.
    auto iter = q.begin();
    std::advance(iter, tag - front_tag);
    return iter;
  } else {
    auto iter = q.end();
    std::advance(iter, tag - back_tag - 1);
    return iter;
  }
}

HHRfNode::HHRfNode(Driver& driver, int f)
  : driver(driver), f(f), dispatched(0) {
}
