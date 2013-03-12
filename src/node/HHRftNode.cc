#include "node/HHRftNode.h"
#include "driver/Driver.h"
#include "packet/packet.h"
#include "util/Math.h"
#include "network/network.h"
#include "logger/logger.h"
#include <utility>
#include <iterator>

HHRftNode* HHRftNode::create(Driver& driver) {
  int f = driver.get_option<int>("node.f");
  int t = driver.get_option<int>("node.t");
  return new HHRftNode(driver, f, t);
}

void HHRftNode::announce_options(Driver& driver) {
  driver.register_option("node.count", po::value<int>(), "count of all nodes");
  driver.register_option("node.f", po::value<int>(), "f for the 2HR-ft");
  driver.register_option("node.t", po::value<int>(), "t for the 2HR-ft");
}

HHRftNode::~HHRftNode() {
}

void HHRftNode::set_tag(int tag) {
  node_tag = tag;
}

int HHRftNode::get_tag() const {
  return node_tag;
}

const IntPos& HHRftNode::get_pos() const {
  return pos;
}

void HHRftNode::set_pos(IntPos& pos) {
  driver.get_logger().node_moved(*this);
  this->pos = pos;
}

void HHRftNode::set_dest(Node& node) {
  dest_node = &node;
}

void HHRftNode::add_packet(PacketPtr&& packet) {
  packet->set_src(*this);
  packet->set_dest(*dest_node);
  packet->set_tag(request_map[dest_node]++);
  packet->get_time_stamp().push_back(driver.get_tick());
  if (waiting_queue.empty()) {
    log_send_start(packet->get_tag(), driver.get_tick());
    packet->get_time_stamp().push_back(driver.get_tick());
  }
  driver.get_logger().packet_generated(*this, *packet);
  waiting_queue.push_back(std::move(packet));
}

HHRftNode& HHRftNode::get_dest() {
  return *dest_node;
}

void HHRftNode::scheduled() {
  Network& network = driver.get_network();
  auto iter_end = network.receiver_end(*this);
  for (auto iter = network.receiver_begin(*this); iter != iter_end; ++iter) {
    if (dest_node == iter.operator->()) {
      deferred_purge_local_queue();
      SD(*iter);
      return;
    }
  }
  auto iter = Network::random_choose(network.receiver_begin(*this), iter_end);
  if (iter != iter_end) {
    if (Math::happen(1, 2)) {
      deferred_purge_local_queue();
      SR(*iter);
    } else {
      deferred_purge_relay_queue(*iter);
      RD(*iter);
    }
  }
}

void HHRftNode::receive(HHRftNode& src, PacketPtr&& packet) {
  packet->get_time_stamp().push_back(driver.get_tick());
  driver.get_logger().packet_transfered(src, *this, *packet);
  if (&packet->get_dest() == this) {  // SD or RD.
    int& r = request_map[this];
    log_receive_end(r, driver.get_tick());
    log_receive_start(++r, driver.get_tick());
    packet = nullptr;
  } else { // SR.
    request_map[&packet->get_dest()] = packet->get_tag() + 1;
    relay_map[&packet->get_dest()].push_back(std::move(packet));
  }
}

void HHRftNode::SD(HHRftNode& dest) {
  int eop = eop_map[&dest];
  int& tag = dest.request_map[&dest];
  if (eop > tag) {
    for (int i = tag + 1; i <= eop; ++i) {
      dest.log_receive_end(i - 1, driver.get_tick());
      dest.log_receive_start(i, driver.get_tick());
    }
    tag = eop;
  }
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
      log_send_end((*iter)->get_tag() - 1, driver.get_tick());
      log_send_start((*iter)->get_tag(), driver.get_tick());
    }
    log_send_end((*iter)->get_tag(), driver.get_tick());
    dest.receive(*this, std::move(*iter));
    dispatched = 0;
    waiting_queue.erase(waiting_queue.begin(), ++iter);
    if (!waiting_queue.empty()) {
      log_send_start(waiting_queue.front()->get_tag(), driver.get_tick());
      waiting_queue.front()->get_time_stamp().push_back(driver.get_tick());
    }
    sent_queue.clear();
  }
}

void HHRftNode::SR(HHRftNode& relay) {
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
    log_send_end(ptr->get_tag(), driver.get_tick());
    sent_queue.splice(sent_queue.end(), waiting_queue, waiting_queue.begin());
    if (!waiting_queue.empty()) {
      log_send_start(waiting_queue.front()->get_tag(), driver.get_tick());
      waiting_queue.front()->get_time_stamp().push_back(driver.get_tick());
    }
  }
}

void HHRftNode::RD(HHRftNode& other_dest) {
  int eop = eop_map[&other_dest];
  int& tag = other_dest.request_map[&other_dest];
  if (eop > tag) {
    for (int i = tag + 1; i <= eop; ++i) {
      other_dest.log_receive_end(i - 1, driver.get_tick());
      other_dest.log_receive_start(i, driver.get_tick());
    }
    tag = eop;
  }
  Queue& relay_queue = relay_map[&other_dest];
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

auto HHRftNode::find_in_sequent_queue(Queue& q, int tag) -> Queue::iterator {
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

void HHRftNode::deferred_purge_local_queue() {
  int& eop = eop_map[dest_node];
  int tick = driver.get_tick();
  // Purge sent queue is easier. Simply purge out of date packets and refresh
  // eop.
  while (!sent_queue.empty()) {
    if (tick - sent_queue.front()->get_time_stamp()[1] > t) {
      eop = sent_queue.front()->get_tag() + 1;
      sent_queue.pop_front();
    } else {
      break;
    }
  }
  // Because many of the packets in waiting queue may have been out of date, we
  // should purge them sequently, and how many packets should be purged should
  // regard to the fact that they should have been timed out during the
  // deferred time one by one.
  while (!waiting_queue.empty()) {
    int front_time = waiting_queue.front()->get_time_stamp()[1];
    if (tick - front_time > t) {  // Front packet should have been purged.
      eop = waiting_queue.front()->get_tag() + 1;
      log_send_end(waiting_queue.front()->get_tag(), front_time + t + 1);
      // Note that we don't put out of date packets into sent queue because we
      // no longer need them. If sent queue is not empty now, we can make the
      // assertion that the front packet in waiting queue is on time.
      waiting_queue.pop_front();
      dispatched = 0;
      if (!waiting_queue.empty()) {
        log_send_start(waiting_queue.front()->get_tag(), front_time + t + 1);
        waiting_queue.front()->get_time_stamp().push_back(front_time + t + 1);
      }
    } else {
      break;
    }
  }
}

void HHRftNode::deferred_purge_relay_queue(HHRftNode& dest) {
  Queue& q = relay_map[&dest];
  int& eop = eop_map[&dest];
  int tick = driver.get_tick();
  while (!q.empty() && tick - q.front()->get_time_stamp()[1] > t) {
    eop = q.front()->get_tag() + 1;
    q.pop_front();
  }
}

void HHRftNode::log_send_start(int tag, int tick) {
  driver.get_logger().log("[%d]\tS.s\t(%d)\n", tick, tag);
}
void HHRftNode::log_send_end(int tag, int tick) {
  driver.get_logger().log("[%d]\tS.e\t(%d)\n", tick, tag);
}

void HHRftNode::log_receive_start(int tag, int tick) {
  driver.get_logger().log("[%d]\tD.s\t(%d)\n", tick, tag);
}

void HHRftNode::log_receive_end(int tag, int tick) {
  driver.get_logger().log("[%d]\tD.e\t(%d)\n", tick, tag);
}

HHRftNode::HHRftNode(Driver& driver, int f, int t)
  : driver(driver), f(f), dispatched(0), t(t) {
}
