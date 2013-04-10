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
  printf("Generated: %d\n", (int) generated.size());
  printf("Dispatched: %d\n", (int) dispatched.size());
  printf("Received: %d\n", (int) received.size());
  std::vector<int> diff;
  std::set_difference(dropped.begin(), dropped.end(), received.begin(),
      received.end(), std::inserter(diff, diff.end()));
  printf("Dropped and not received: %d\n", (int) diff.size());
  if (!dispatched.empty()) {
    printf("Received / Dispatched = %lf\n",
        double(received.size()) / dispatched.size());
  }
  if  (!received.empty()) {
    printf("Average delay: %ld\n", total_delay / (long) received.size());
    printf("Average deliver delay: %ld\n",
        total_deliver_delay / (long) received.size());
  }
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
  generated.insert(packet.get_tag());
}

void HHRObserver::packet_transfered(Node& from, Node& to, Packet& packet) {
  if (driver.get_tick() < starting_tick) {
    return;
  }
  if (&from == &packet.get_src() && &to != &packet.get_dest()) {  // SR.
    dispatched.insert(packet.get_tag());
  } else {  // SD or RD.
    auto& time_stamp = packet.get_time_stamp();
    if (&from == &packet.get_src()) {  // SD.
      dispatched.insert(packet.get_tag());
      total_delay += time_stamp[2] - time_stamp[0];
      total_deliver_delay += time_stamp[2] - time_stamp[1];
    } else {  // RD.
      total_delay += time_stamp[3] - time_stamp[0];
      total_deliver_delay += time_stamp[3] - time_stamp[1];
    }
    received.insert(packet.get_tag());
  }
}

void HHRObserver::packet_dropped(Node& where, Packet& packet) {
  if (driver.get_tick() < starting_tick) {
    return;
  }
  dropped.insert(packet.get_tag());
}

HHRObserver::HHRObserver(Driver& driver, double ratio)
  : driver(driver), total_delay(0), total_deliver_delay(0) {
  starting_tick = int(driver.get_total_loop() * (1 - ratio));
}
