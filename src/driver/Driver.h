#ifndef DRIVER_DRIVER_H_
#define DRIVER_DRIVER_H_

#include <boost/program_options.hpp>
#include <string>
#include <iostream>
#include <cstdlib>

namespace po = boost::program_options;

class Driver {
  
  public:

    Driver();

    template<typename T>
    T get_opt(const std::string& name) const {
      using namespace std;
      try {
        return opt_map[name].as<T>();
      } catch (exception& e) {
        cerr << "misconfiguration " << name << endl;
        exit(1);
      }
    }
    
    bool init(int argc, char* argv[]);

    void init_components();

    void prepare_components();
    
    template<typename...T>
    void register_option(T...opt) {
      config_opt.add_options()(opt...);
    }

  private:

    template<typename...T>
    void register_generic_option(T...opt) {
      generic_opt.add_options()(opt...);
    }

    po::options_description generic_opt;
    po::options_description config_opt;
    po::options_description cmd_opt;
    po::variables_map opt_map;

};

#endif
