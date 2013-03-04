#include "logger/Printer.h"
#include "driver/Driver.h"

Printer* Printer::create(Driver& driver) {
  Switch c;
  c.log_msg = driver.get_option<bool>("logger.log_msg");
  c.log_before_simulation = driver.get_option<bool>(
      "logger.log_before_simulation");
  c.log_after_simulation = driver.get_option<bool>(
      "logger.log_after_simulation");
  c.log_before_loop = driver.get_option<bool>( "logger.log_before_loop");
  c.log_after_loop = driver.get_option<bool>("logger.log_after_loop");
  c.log_node_moved = driver.get_option<bool>("logger.log_node_moved");
  c.log_packet_generated = driver.get_option<bool>(
      "logger.log_packet_generated");
  c.log_packet_transfered = driver.get_option<bool>(
      "logger.log_packet_transfered");
  return new Printer(driver, c);
};

void Printer::announce_options(Driver& driver) {
  driver.register_option("logger.log_msg", po::value<bool>(),
      "whether to log user-defined messages");
  driver.register_option("logger.log_before_simulation", po::value<bool>(),
      "whether to log before simulation");
  driver.register_option("logger.log_after_simulation", po::value<bool>(),
      "whether to log after simulation");
  driver.register_option("logger.log_before_loop", po::value<bool>(),
      "whether to log before each loop");
  driver.register_option("logger.log_after_loop", po::value<bool>(),
      "whether to log after each loop");
  driver.register_option("logger.log_node_moved", po::value<bool>(),
      "whether to log when node is moved");
  driver.register_option("logger.log_packet_generated", po::value<bool>(),
      "whether to log when packet is generated");
  driver.register_option("logger.log_packet_transfered", po::value<bool>(),
      "whether to log when packet is transfered");
}

void Printer::before_simulation() {

}

void Printer::after_simulation() {

}

void Printer::before_loop() {

}

void Printer::after_loop() {

}

void Printer::node_moved(Node& node) {

}

void Printer::packet_generated(Node& where, Packet& pack) {

}

void Printer::packet_transfered(Node& from, Node& to, Packet& pack) {

}

Printer::Printer(Driver& driver, const Switch& control)
  : control(control), driver(driver) {
}
