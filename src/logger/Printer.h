#ifndef LOGGER_PRINTER_H_
#define LOGGER_PRINTER_H_

/**
 * @file logger/Printer.h
 * @brief Header for Printer.
 */

#include "driver/types.h"
#include "driver/Driver.h"
#include <cstdio>

/**
 * @brief Printer that prints readable information.
 * @ingroup logger
 */
class Printer {

  public:

    /**
     * @brief Create instance.
     * @return The instance that should be deleted by user.
     */
    static Printer* create(Driver& driver);

    /**
     * @brief Announce the options that is recognized by the class.
     */
    static void announce_options(Driver& driver);

    /**
     * @brief Accustomed log with c-style format.
     */
    template<typename...T>
    void log(const char* format, T...arg) const {
      if (control.log_msg) {
        printf("[%d] ", driver.get_tick());
        printf(format, arg...);
      }
    }

    /**
     * @brief Called before simulation.
     */
    void before_simulation();

    /**
     * @brief Called after simulation.
     */
    void after_simulation();

    /**
     * @brief Called before each simulation loop.
     */
    void before_loop();

    /**
     * @brief Called after each simulation loop.
     */
    void after_loop();

    /**
     * @brief Called after a node's position is refreshed.
     */
    void node_moved(Node& node);

    /**
     * @brief Called after a packet is generated at a certain node.
     */
    void packet_generated(Node& where, Packet& packet);

    /**
     * @brief Called when a packet is being transfered from a node to another.
     */
    void packet_transfered(Node& from, Node& to, Packet& packet);

    /**
     * @brief Called when a packet is dropped intentionally.
     */
    void packet_dropped(Node& where, Packet& packet);

  private:

    typedef struct {
      bool log_msg;
      bool log_before_simulation;
      bool log_after_simulation;
      bool log_before_loop;
      bool log_after_loop;
      bool log_node_moved;
      bool log_packet_generated;
      bool log_packet_transfered;
      bool log_packet_dropped;
    } Switch;

    Printer(Driver& driver, const Switch& control);
    Switch control;
    Driver& driver;

};

#endif
