#ifndef LOGGER_DATA_H_
#define LOGGER_DATA_H_

/**
 * @file logger/Data.h
 * @brief Header for Data.
 */

#include "driver/types.h"
#include <cstdio>

/**
 * @brief Accustomed data analyzer.
 * @ingroup logger
 */
class Data {

  public:

    /**
     * @brief Create instance.
     * @return The instance that should be deleted by user.
     */
    static Data* create(Driver& driver);

    /**
     * @brief Announce the options that is recognized by the class.
     */
    static void announce_options(Driver& driver);

    /**
     * @brief Accustomed log with c-style format.
     */
    template<typename...T>
    void log(const char* format, T...arg) const {
      printf(format, arg...);
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

    Data(Driver& driver);

    Driver& driver;

};

#endif 
