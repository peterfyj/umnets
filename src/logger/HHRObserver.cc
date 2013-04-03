#include "logger/HHRObserver.h"
#include "driver/Driver.h"
#include "packet/packet.h"
#include "node/node.h"
#include <algorithm>

HHRObserver* HHRObserver::create(Driver& driver) {
  double ratio = driver.get_option<double>("logger.observe_ratio");
  return new HHRObserver(driver, ratio);
}

void HHRObserver::announce_options(Driver& driver) {
  driver.register_option("logger.observe_ratio", po::value<double>(), 
      "time ratio for observing network events, end aligned");
}

void HHRObserver::before_simulation() {
  printf("Lambda: %lf\n", driver.get_option<double>("traffic.lambda"));
}

void HHRObserver::after_simulation() {
  printf("Generated: %d\n", generated);
  printf("Dispatched: %d\n", dispatched);
  printf("Received: %d\n", received);
  printf("Average delay: %ld\n", total_delay / received);
  printf("Average deliver delay: %ld\n", total_deliver_delay / received);
}

void HHRObserver::before_loop() {
}

void HHRObserver::after_loop() {
}

void HHRObserver::node_moved(Node& node) {
}

void HHRObserver::packet_generated(Node& where, Packet& packet) {
  if (driver.get_tick() < starting_tick) {
    return;
  }
  ++generated;
}

void HHRObserver::packet_transfered(Node& from, Node& to, Packet& packet) {
  if (driver.get_tick() < starting_tick) {
    return;
  }
  if (&from == &packet.get_src() && &to != &packet.get_dest()) {  // SR.
    if (packet.get_tag() >= next_dispatch) {
      ++dispatched;
      next_dispatch = packet.get_tag() + 1;
    }
  } else {  // SD or RD.
    auto& time_stamp = packet.get_time_stamp();
    if (&from == &packet.get_src()) {  // SD.
      total_delay += time_stamp[2] - time_stamp[0];
      total_deliver_delay += time_stamp[2] - time_stamp[1];
    } else {  // RD.
      total_delay += time_stamp[3] - time_stamp[0];
      total_deliver_delay += time_stamp[3] - time_stamp[1];
    }
    ++received;
  }
}

HHRObserver::HHRObserver(Driver& driver, double ratio)
  : driver(driver), generated(0), dispatched(0), received(0)
  , next_dispatch(0), total_delay(0), total_deliver_delay(0) {
  starting_tick = int(driver.get_total_loop() * (1 - ratio));
}
