#include "logger/ServiceObserver.h"
#include "driver/Driver.h"
#include "packet/packet.h"
#include "node/node.h"
#include <algorithm>

ServiceObserver* ServiceObserver::create(Driver& driver) {
  double ratio = driver.get_option<double>("logger.observe_ratio");
  return new ServiceObserver(driver, ratio);
}

void ServiceObserver::announce_options(Driver& driver) {
  driver.register_option("logger.observe_ratio", po::value<double>(), 
      "time ratio for observing network events, end aligned");
}

void ServiceObserver::before_simulation() {
  printf("Lambda: %lf\n", driver.get_option<double>("traffic.lambda"));
}

void ServiceObserver::after_simulation() {
  int late = 0;
  int all = (int) std::min(send_time.size(), receive_time.size()) - 1;
  int start = 0;
  while (start < all && send_time[start].start < starting_tick) {
    ++start;
  }
  for (int i = start; i < all; ++i) {
    if (send_time[i].end < receive_time[i].start) {
      ++late;
    }
  }
  printf("Generated: %d\n", generated);
  printf("Dispatched: %d\n", dispatched);
  printf("Received: %d\n", received);
  printf("Receive ratio: %lf\n", double(received) / dispatched);
  printf("Late: %d / %d\n", late, all - start);
  printf("Late ratio: %lf\n", double(late) / (all - start));
}

void ServiceObserver::before_loop() {
  // TODO
}

void ServiceObserver::after_loop() {
  // TODO
}

void ServiceObserver::node_moved(Node& node) {
  // TODO
}

void ServiceObserver::packet_generated(Node& where, Packet& packet) {
  if (driver.get_tick() < starting_tick) {
    return;
  }
  ++generated;
}

void ServiceObserver::packet_transfered(Node& from, Node& to, Packet& packet) {
  if (driver.get_tick() < starting_tick) {
    return;
  }
  if (&from == &packet.get_src() && &to != &packet.get_dest()) {  // SR.
    if (packet.get_tag() > last_dispatch) {
      ++dispatched;
      last_dispatch = packet.get_tag();
    }
  } else {  // SD or RD.
    ++received;
  }
}

void ServiceObserver::log_send_start(int tag, int tick) {
  send_time.push_back(Service(tick, 0));
}

void ServiceObserver::log_send_end(int tag, int tick) {
  send_time.back().end = tick;
}

void ServiceObserver::log_receive_start(int tag, int tick) {
  receive_time.push_back(Service(tick, 0));
}

void ServiceObserver::log_receive_end(int tag, int tick) {
  receive_time.back().end = tick;
}


ServiceObserver::ServiceObserver(Driver& driver, double ratio)
  : driver(driver), generated(0), dispatched(0), received(0)
  , last_dispatch(-1) {
  starting_tick = int(driver.get_total_loop() * (1 - ratio));
}
