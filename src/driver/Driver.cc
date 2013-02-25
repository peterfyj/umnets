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

Driver::Driver() : generic_opt(options_description("Generic options"))
                 , config_opt(options_description("Configuration")) {
  register_generic_option("version,v", "print version");
  register_generic_option("help,h", "print help");
  register_generic_option("config,c", 
      value<string>()->default_value("config.cfg"), "configuration file");
  register_option("driver.loop", value<int>(), "loop times of the simulation");
}

bool Driver::init(int argc, char* argv[]) {
  try {
    cmd_opt.add(generic_opt).add(config_opt);
    store(parse_command_line(argc, argv, cmd_opt), opt_map); 
    notify(opt_map);
    if (opt_map.count("version")) {
      cout << "umnets v2.0" << endl;
      cout << "contact author: peterfyj@gmail.com" << endl;
      exit(0);
    } else if (opt_map.count("help")) {
      cout << generic_opt << endl;
      cout << config_opt << endl;
      exit(0);
    } else if (opt_map.count("config")) {
      string filename = opt_map["config"].as<string>();
      ifstream fin(filename.c_str());
      if (fin) {
        store(parse_config_file(fin, config_opt), opt_map);
        notify(opt_map);
      } else {
        cerr << "failed to open " << filename << endl;
        return false;
      }
    }
  } catch (exception& e) {
    cerr << e.what() << endl;
    return false;
  }
  return true;
}

void Driver::init_components() {
}

void Driver::prepare_components() {
  logger_t::announce_options(*this);
  motion_t::announce_options(*this);
  network_t::announce_options(*this);
  node_t::announce_options(*this);
  packet_t::announce_options(*this);
  scheduler_t::announce_options(*this);
  traffic_t::announce_options(*this);
}
