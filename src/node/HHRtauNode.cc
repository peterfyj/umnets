#include "node/HHRtauNode.h"
#include "driver/Driver.h"
#include "packet/packet.h"
#include "util/Math.h"
#include "network/network.h"
#include "logger/logger.h"
#include <utility>
#include <iterator>

HHRtauNode* HHRtauNode::create(Driver& driver) {
  int tau = driver.get_option<int>("node.tau");
  int beta = driver.get_option<int>("node.beta");
  int beta_local = driver.get_option<int>("node.beta_local");
  double q = driver.get_option<double>("node.q");
  return new HHRtauNode(driver, tau, beta, beta_local, q);
}

void HHRtauNode::announce_options(Driver& driver) {
  driver.register_option("node.count", po::value<int>(), "count of all nodes");
  driver.register_option("node.tau", po::value<int>(), "time threshold");
  driver.register_option("node.beta", po::value<int>(), "relay buffer limit");
  driver.register_option("node.beta_local", po::value<int>(), 
      "relay buffer limit");
  driver.register_option("node.q", po::value<double>(), "S-R probability");
}

HHRtauNode::~HHRtauNode() {
}

void HHRtauNode::set_tag(int tag) {
  node_tag = tag;
}

int HHRtauNode::get_tag() const {
  return node_tag;
}

const IntPos& HHRtauNode::get_pos() const {
  return pos;
}

void HHRtauNode::set_pos(IntPos& pos) {
  driver.get_logger().node_moved(*this);
  this->pos = pos;
}

void HHRtauNode::set_dest(HHRtauNode& node) {
  dest_node = &node;
}

void HHRtauNode::add_packet(PacketPtr&& packet) {
  packet->set_src(*this);
  packet->set_dest(*dest_node);
  packet->set_tag(seq++);
  packet->get_time_stamp().push_back(driver.get_tick());
  auto& local_queue = queue_map[dest_node];
  driver.get_logger().packet_generated(*this, *packet);
  if ((int) local_queue.size() >= beta_local) {
    packet->get_time_stamp().push_back(driver.get_tick());
    driver.get_logger().packet_dropped(*this, *packet);
    packet = nullptr;
    return;
  }
  if (local_queue.empty()) {
    packet->get_time_stamp().push_back(driver.get_tick());
    token_map[dest_node] = driver.register_time_out(driver.get_tick() + tau,
        get_local_time_out_callback());
  }
  local_queue.push_back(std::move(packet));
}

HHRtauNode& HHRtauNode::get_dest() {
  return *dest_node;
}

void HHRtauNode::scheduled() {
  Network& network = driver.get_network();
  auto iter_end = network.receiver_end(*this);
  for (auto iter = network.receiver_begin(*this); iter != iter_end; ++iter) {
    if (dest_node == &*iter) {
      SD(*iter);
      return;
    }
  }
  if (Math::happen(q)) {
    SR();
  } else {
    auto ptr = Network::random_choose(network.receiver_begin(*this), iter_end);
    if (ptr != iter_end) {
      RD(*ptr);
    }
  }
}

void HHRtauNode::receive(HHRtauNode& src, PacketPtr&& packet) {
  packet->get_time_stamp().push_back(driver.get_tick());
  driver.get_logger().packet_transfered(src, *this, *packet);
  if (&packet->get_dest() == this) {  // SD or RD.
    packet = nullptr;
  } else { // SR.
    auto& relay_queue = queue_map[&packet->get_dest()];
    if ((int) relay_queue.size() < beta) {
      if (relay_queue.empty()) {
        token_map[&packet->get_dest()] = driver.register_time_out(
           packet->get_time_stamp()[1] + tau,
           get_relay_time_out_callback(packet->get_dest()));
      }
      relay_queue.push_back(std::move(packet));
    } else {
      driver.get_logger().packet_dropped(*this, *packet);
      packet = nullptr;
    }
  }
}

HHRtauNode::HHRtauNode(Driver& driver, int tau, int beta, int beta_local, 
    double q)
  : driver(driver), tau(tau), beta(beta), beta_local(beta_local)
  , q(q), seq(0) {
}

Driver::Callback HHRtauNode::get_local_time_out_callback() {
  return [this] () {
    auto& queue = queue_map[dest_node];
    auto& token = token_map[dest_node];
    queue.front()->get_time_stamp().push_back(driver.get_tick());
    driver.get_logger().packet_dropped(*this, *queue.front());
    queue.pop_front();
    if (!queue.empty()) {
      queue.front()->get_time_stamp().push_back(driver.get_tick());
      token = driver.register_time_out(driver.get_tick() + tau,
          get_local_time_out_callback());
    }
  };
}

Driver::Callback HHRtauNode::get_relay_time_out_callback(HHRtauNode& dest) {
  return [&dest, this] () {
    auto& queue = queue_map[&dest];
    auto& token = token_map[&dest];
    queue.front()->get_time_stamp().push_back(driver.get_tick());
    driver.get_logger().packet_dropped(*this, *queue.front());
    queue.pop_front();
    if (!queue.empty()) {
      int stamp = queue.front()->get_time_stamp()[1];
      token = driver.register_time_out(stamp + tau,
          get_relay_time_out_callback(dest));
    }
  };
}

void HHRtauNode::SD(HHRtauNode& dest) {
  auto& local_queue = queue_map[&dest];
  if (local_queue.empty()) {
    return;
  }
  driver.unregister_time_out(token_map[&dest]);
  dest.receive(*this, std::move(local_queue.front()));
  local_queue.pop_front();
  for (int i = 0; !local_queue.empty() && i < beta - 1; ++i) {
    local_queue.front()->get_time_stamp().push_back(driver.get_tick());
    dest.receive(*this, std::move(local_queue.front()));
    local_queue.pop_front();
  }
  if (!local_queue.empty()) {
    local_queue.front()->get_time_stamp().push_back(driver.get_tick() + 1);
    token_map[&dest] = driver.register_time_out(driver.get_tick() + tau + 1,
        get_local_time_out_callback());
  }
}

void HHRtauNode::SR() {
  auto& local_queue = queue_map[dest_node];
  if (local_queue.empty()) {
    return;
  }
  driver.unregister_time_out(token_map[dest_node]);
  auto& network = driver.get_network();
  auto iter_end = network.receiver_end(*this);
  for (auto iter = network.receiver_begin(*this); iter != iter_end; ++iter) {
    iter->receive(*this, PacketPtr(local_queue.front()->clone()));
  }
  local_queue.pop_front();
  if (!local_queue.empty()) {
    local_queue.front()->get_time_stamp().push_back(driver.get_tick() + 1);
    token_map[dest_node] = driver.register_time_out(
        driver.get_tick() + tau + 1, get_local_time_out_callback());
  }
}

void HHRtauNode::RD(HHRtauNode& other_dest) {
  auto& relay_queue = queue_map[&other_dest];
  if (!relay_queue.empty()) {
    driver.unregister_time_out(token_map[&other_dest]);
  }
  while (!relay_queue.empty()) {
    other_dest.receive(*this, std::move(relay_queue.front()));
    relay_queue.pop_front();
  }
}
