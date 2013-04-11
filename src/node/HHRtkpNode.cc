#include "node/HHRtkpNode.h"
#include "driver/Driver.h"
#include "packet/packet.h"
#include "util/Math.h"
#include "network/network.h"
#include "logger/logger.h"
#include <utility>
#include <iterator>

HHRtkpNode* HHRtkpNode::create(Driver& driver) {
  int t = driver.get_option<int>("node.t");
  int k = driver.get_option<int>("node.k");
  double p = driver.get_option<double>("node.p");
  return new HHRtkpNode(driver, t, k, p);
}

void HHRtkpNode::announce_options(Driver& driver) {
  driver.register_option("node.count", po::value<int>(), "count of all nodes");
  driver.register_option("node.t", po::value<int>(), "time threshold");
  driver.register_option("node.k", po::value<int>(), "relay buffer limit");
  driver.register_option("node.p", po::value<double>(), "S-R probability");
}

HHRtkpNode::~HHRtkpNode() {
}

void HHRtkpNode::set_tag(int tag) {
  node_tag = tag;
}

int HHRtkpNode::get_tag() const {
  return node_tag;
}

const IntPos& HHRtkpNode::get_pos() const {
  return pos;
}

void HHRtkpNode::set_pos(IntPos& pos) {
  driver.get_logger().node_moved(*this);
  this->pos = pos;
}

void HHRtkpNode::set_dest(HHRtkpNode& node) {
  dest_node = &node;
}

void HHRtkpNode::add_packet(PacketPtr&& packet) {
  packet->set_src(*this);
  packet->set_dest(*dest_node);
  packet->set_tag(seq++);
  packet->get_time_stamp().push_back(driver.get_tick());
  auto& local_queue = queue_map[dest_node];
  driver.get_logger().packet_generated(*this, *packet);
  if (local_queue.empty()) {
    packet->get_time_stamp().push_back(driver.get_tick());
    token_map[dest_node] = driver.register_time_out(driver.get_tick() + t,
        get_local_time_out_callback());
  }
  local_queue.push_back(std::move(packet));
}

HHRtkpNode& HHRtkpNode::get_dest() {
  return *dest_node;
}

void HHRtkpNode::scheduled() {
  Network& network = driver.get_network();
  auto iter_end = network.receiver_end(*this);
  for (auto iter = network.receiver_begin(*this); iter != iter_end; ++iter) {
    if (dest_node == &*iter) {
      SD(*iter);
      return;
    }
  }
  if (Math::happen(p)) {
    SR();
  } else {
    auto ptr = Network::random_choose(network.receiver_begin(*this), iter_end);
    if (ptr != iter_end) {
      RD(*ptr);
    }
  }
}

void HHRtkpNode::receive(HHRtkpNode& src, PacketPtr&& packet) {
  packet->get_time_stamp().push_back(driver.get_tick());
  driver.get_logger().packet_transfered(src, *this, *packet);
  if (&packet->get_dest() == this) {  // SD or RD.
    packet = nullptr;
  } else { // SR.
    auto& relay_queue = queue_map[&packet->get_dest()];
    if ((int) relay_queue.size() < k) {
      if (relay_queue.empty()) {
        token_map[&packet->get_dest()] = driver.register_time_out(
           packet->get_time_stamp()[1] + t,
           get_relay_time_out_callback(packet->get_dest()));
      }
      relay_queue.push_back(std::move(packet));
    } else {
      packet = nullptr;
    }
  }
}

HHRtkpNode::HHRtkpNode(Driver& driver, int t, int k, double p)
  : driver(driver), t(t), k(k), p(p), seq(0) {
}

Driver::Callback HHRtkpNode::get_local_time_out_callback() {
  return [this] () {
    auto& queue = queue_map[dest_node];
    auto& token = token_map[dest_node];
    queue.front()->get_time_stamp().push_back(driver.get_tick());
    driver.get_logger().packet_dropped(*this, *queue.front());
    queue.pop_front();
    if (!queue.empty()) {
      queue.front()->get_time_stamp().push_back(driver.get_tick());
      token = driver.register_time_out(driver.get_tick() + t,
          get_local_time_out_callback());
    }
  };
}

Driver::Callback HHRtkpNode::get_relay_time_out_callback(HHRtkpNode& dest) {
  return [&dest, this] () {
    auto& queue = queue_map[&dest];
    auto& token = token_map[&dest];
    queue.front()->get_time_stamp().push_back(driver.get_tick());
    driver.get_logger().packet_dropped(*this, *queue.front());
    queue.pop_front();
    if (!queue.empty()) {
      int stamp = queue.front()->get_time_stamp()[1];
      token = driver.register_time_out(stamp + t,
          get_relay_time_out_callback(dest));
    }
  };
}

void HHRtkpNode::SD(HHRtkpNode& dest) {
  auto& local_queue = queue_map[&dest];
  if (!local_queue.empty()) {
    driver.unregister_time_out(token_map[&dest]);
    dest.receive(*this, std::move(local_queue.front()));
    local_queue.pop_front();
  }
  for (int i = 0; !local_queue.empty() && i < k; ++i) {
    local_queue.front()->get_time_stamp().push_back(driver.get_tick());
    dest.receive(*this, std::move(local_queue.front()));
    local_queue.pop_front();
  }
  if (!local_queue.empty()) {
    local_queue.front()->get_time_stamp().push_back(driver.get_tick() + 1);
    token_map[&dest] = driver.register_time_out(driver.get_tick() + t + 1,
        get_local_time_out_callback());
  }
}

void HHRtkpNode::SR() {
  auto& local_queue = queue_map[dest_node];
  if (local_queue.empty()) {
    return;
  }
  auto& network = driver.get_network();
  auto iter = network.receiver_begin(*this);
  auto iter_end = network.receiver_end(*this);
  if (iter == iter_end) {
    return;
  }
  driver.unregister_time_out(token_map[dest_node]);
  for (; iter != iter_end; ++iter) {
    iter->receive(*this, PacketPtr(local_queue.front()->clone()));
  }
  local_queue.pop_front();
  if (!local_queue.empty()) {
    local_queue.front()->get_time_stamp().push_back(driver.get_tick() + 1);
    token_map[dest_node] = driver.register_time_out(driver.get_tick() + t + 1,
        get_local_time_out_callback());
  }
}

void HHRtkpNode::RD(HHRtkpNode& other_dest) {
  auto& relay_queue = queue_map[&other_dest];
  if (!relay_queue.empty()) {
    driver.unregister_time_out(token_map[&other_dest]);
  }
  while (!relay_queue.empty()) {
    other_dest.receive(*this, std::move(relay_queue.front()));
    relay_queue.pop_front();
  }
}
