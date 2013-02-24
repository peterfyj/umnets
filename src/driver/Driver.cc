#include "driver/Driver.h"
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
}

bool Driver::init(int argc, char* argv[]) {
  try {
    store(parse_command_line(argc, argv, generic_opt), opt_map); 
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
