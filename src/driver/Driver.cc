#include "driver/Driver.h"
#include "driver/types.h"
#include "logger/logger.h"
#include "motion/motion.h"
#include "network/network.h"
#include "node/node.h"
#include "packet/packet.h"
#include "scheduler/scheduler.h"
#include "traffic/traffic.h"
#include <iostream>
#include <string>
#include <fstream>

using namespace std;
using namespace po;

Driver::Driver() : config_opt(options_description("Configuration"))
                 , generic_opt(options_description("Generic options")) {
  register_generic_option("version,v", "print version");
  register_generic_option("help,h", "print help");
  register_generic_option("config,c", 
      value<string>()->default_value("config.cfg"), "configuration file");
  register_option("driver.loop", value<int>(), "loop times of the simulation");
}

Driver::~Driver() {
}

int Driver::init(int argc, char* argv[]) {
  try {
    cmd_opt.add(generic_opt).add(config_opt);
    store(parse_command_line(argc, argv, cmd_opt), opt_map); 
    notify(opt_map);
    if (opt_map.count("version")) {
      cout << "umnets v2.0" << endl;
      cout << "contact author: peterfyj@gmail.com" << endl;
      return 0;
    } else if (opt_map.count("help")) {
      cout << generic_opt << endl;
      cout << config_opt << endl;
      return 0;
    } else if (opt_map.count("config")) {
      string filename = opt_map["config"].as<string>();
      ifstream fin(filename.c_str());
      if (fin) {
        store(parse_config_file(fin, config_opt), opt_map);
        notify(opt_map);
        return -1;
      } else {
        cerr << "failed to open " << filename << endl;
        return 1;
      }
    }
  } catch (exception& e) {
    cerr << e.what() << endl;
    return 1;
  }
  return -1;
}

void Driver::init_components() {
  network = NetworkPtr(Network::create(*this));
  logger = LoggerPtr(Logger::create(*this));
  motion = MotionPtr(Motion::create(*this));
  scheduler = SchedulerPtr(Scheduler::create(*this));
  int node_count = get_option<int>("node.count");
  for (int i = 0; i < node_count; ++i) {
    network->add_node(*motion, NodePtr(Node::create(*this, i)));
  }
  traffic = TrafficPtr(Traffic::create(*this));
  traffic->create_traffic();
}

void Driver::prepare_components() {
  Logger::announce_options(*this);
  Motion::announce_options(*this);
  Network::announce_options(*this);
  Node::announce_options(*this);
  Packet::announce_options(*this);
  Scheduler::announce_options(*this);
  Traffic::announce_options(*this);
}

Logger& Driver::get_logger() {
  return *logger;
}

Motion& Driver::get_motion() {
  return *motion;
}

Network& Driver::get_network() {
  return *network;
}

Scheduler& Driver::get_scheduler() {
  return *scheduler;
}

Traffic& Driver::get_traffic() {
  return *traffic;
}
