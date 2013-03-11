#ifndef DRIVER_DRIVER_H_
#define DRIVER_DRIVER_H_

/**
 * @file driver/Driver.h
 * @brief Header for Driver.
 */

#include "driver/types.h"
#include <boost/program_options.hpp>
#include <string>
#include <iostream>
#include <cstdlib>

namespace po = boost::program_options;

/**
 * @brief Class that drives the whole simulation process.
 */
class Driver {
  
  public:

    /**
     * @brief Default constructor.
     */
    Driver();

    /**
     * @brief Default destructor.
     */
    ~Driver();

    /**
     * @brief Get a certain option.
     * @tparam T The option type.
     * @param name The option name.
     *
     * The option is defined by the configuration file and could be overwritten
     * from the command line.
     */
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
    
    /**
     * @brief Initialize the driver with given command line arguments.
     */
    int init(int argc, char* argv[]);

    /**
     * @brief Instantiate all the instances needed for the simulation.
     */
    void init_components();

    /**
     * @brief Prepare for components instantiation.
     * 
     * This process allows each components to announce its needed option names,
     * and thus should be called before @ref init_components and @ref init.
     */
    void prepare_components();

    /**
     * @brief Get @ref logger instance.
     */
    Logger& get_logger();

    /**
     * @brief Get @ref motion instance.
     */
    Motion& get_motion();

    /**
     * @brief Get @ref network instance.
     */
    Network& get_network();

    /**
     * @brief Get @ref scheduler instance.
     */
    Scheduler& get_scheduler();

    /**
     * @brief Get @ref traffic instance.
     */
    Traffic& get_traffic();

    /**
     * @brief Start the simulation.
     */
    void start();

    /**
     * @brief Get the current tick count.
     */
    int get_tick();

    /**
     * @brief Called when a component needs to announce a needed option.
     */
    template<typename...T>
    void register_option(T...opt) {
      config_opt.add_options()(opt...);
    }

  private:

    template<typename...T>
    void register_generic_option(T...opt) {
      generic_opt.add_options()(opt...);
    }

    void tick_loop();

    po::options_description cmd_opt;
    po::options_description config_opt;
    po::options_description generic_opt;
    po::variables_map opt_map;
    LoggerPtr logger;
    MotionPtr motion;
    NetworkPtr network;
    SchedulerPtr scheduler;
    TrafficPtr traffic;
    int tick;

};

#endif
