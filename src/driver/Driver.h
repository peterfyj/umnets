#ifndef DRIVER_DRIVER_H_
#define DRIVER_DRIVER_H_

#include "driver/types.h"
#include <boost/program_options.hpp>
#include <string>
#include <iostream>
#include <cstdlib>

namespace po = boost::program_options;

class Driver {
  
  public:

    Driver();
    ~Driver();

    template<typename T>
    T get_option(const std::string& name) const {
      using namespace std;
      try {
        return opt_map[name].as<T>();
      } catch (exception& e) {
        cerr << "misconfiguration " << name << endl;
        exit(1);
      }
    }
    
    int init(int argc, char* argv[]);
    void init_components();
    void prepare_components();
    Logger& get_logger();
    Motion& get_motion();
    Network& get_network();
    Scheduler& get_scheduler();
    Traffic& get_traffic();

    
    template<typename...T>
    void register_option(T...opt) {
      config_opt.add_options()(opt...);
    }

  private:

    template<typename...T>
    void register_generic_option(T...opt) {
      generic_opt.add_options()(opt...);
    }

    po::options_description cmd_opt;
    po::options_description config_opt;
    po::options_description generic_opt;
    po::variables_map opt_map;

    LoggerPtr logger;
    MotionPtr motion;
    NetworkPtr network;
    SchedulerPtr scheduler;
    TrafficPtr traffic;

};

#endif
