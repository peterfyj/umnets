#include "logger/Printer.h"
#include "driver/Driver.h"
#include "node/node.h"
#include "packet/packet.h"

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
  if (!control.log_before_simulation) {
    return;
  }
  printf("[%d] starting simulation.\n", driver.get_tick());
}

void Printer::after_simulation() {
  if (!control.log_after_simulation) {
    return;
  }
  printf("[%d] simulation is over.\n", driver.get_tick());
}

void Printer::before_loop() {
  if (!control.log_before_loop) {
    return;
  }
  printf("[%d] staring tick.\n", driver.get_tick());
}

void Printer::after_loop() {
  if (!control.log_after_loop) {
    return;
  }
  printf("[%d] tick is over.\n", driver.get_tick());
}

void Printer::node_moved(Node& node) {
  if (!control.log_node_moved) {
    return;
  }
  printf("[%d] (%d) move to (%d, %d)\n", driver.get_tick(), node.get_tag(),
      node.get_pos().first, node.get_pos().second);
}

void Printer::packet_generated(Node& where, Packet& packet) {
  if (!control.log_packet_generated) {
    return;
  }
  printf("[%d] packet %d (%d)->(%d): generated at (%d)\n",
      driver.get_tick(), packet.get_tag(), packet.get_src().get_tag(),
      packet.get_dest().get_tag(), where.get_tag());
}

void Printer::packet_transfered(Node& from, Node& to, Packet& packet) {
  if (!control.log_packet_transfered) {
    return;
  }
  static const char* sd = "SD";
  static const char* rd = "RD";
  static const char* sr = "SR";
  const char* now;
  if (&packet.get_dest() == &to) {  // SD or RD.
    if (&packet.get_src() == &from) {  // SD.
      now = sd;
    } else {  // RD.
      now = rd;
    }
  } else { // SR
    now = sr;
  }
  printf("[%d] packet %d (%d)->(%d): %s (%d)->(%d)\n",
      driver.get_tick(), packet.get_tag(), packet.get_src().get_tag(),
      packet.get_dest().get_tag(), now, from.get_tag(), to.get_tag());
}

Printer::Printer(Driver& driver, const Switch& control)
  : control(control), driver(driver) {
}
