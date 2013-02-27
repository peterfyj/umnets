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
  c.log_packet_dispatched = driver.get_option<bool>(
      "logger.log_packet_dispatched");
  c.log_packet_relayed = driver.get_option<bool>(
      "logger.log_packet_relayed");
  c.log_packet_received = driver.get_option<bool>(
      "logger.log_packet_received");
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
  driver.register_option("logger.log_packet_dispatched", po::value<bool>(),
      "whether to log when packet is dispatched");
  driver.register_option("logger.log_packet_relayed", po::value<bool>(),
      "whether to log when packet is relayed");
  driver.register_option("logger.log_packet_received", po::value<bool>(),
      "whether to log when packet is received");
}

Printer::Printer(Driver& driver, const Switch& control)
  : control(control), driver(driver) {
}
