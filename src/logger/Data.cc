#include "logger/Data.h"

Data* Data::create(Driver& driver) {
  return new Data(driver);
}

void Data::announce_options(Driver& driver) {
  // TODO
}

void Data::before_simulation() {
  // TODO
}

void Data::after_simulation() {
  // TODO
}

void Data::before_loop() {
  // TODO
}

void Data::after_loop() {
  // TODO
}

void Data::node_moved(Node& node) {
  // TODO
}

void Data::packet_generated(Node& where, Packet& packet) {
  // TODO
}

void Data::packet_transfered(Node& from, Node& to, Packet& packet) {
  // TODO
}

void Data::packet_dropped(Node& where, Packet& packet) {
  // TODO
}

Data::Data(Driver& driver) : driver(driver) {
}
