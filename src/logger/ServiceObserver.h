#ifndef LOGGER_SERVICEOBSERVER_H_
#define LOGGER_SERVICEOBSERVER_H_

/**
 * @file logger/ServiceObserver.h
 * @brief Header for ServiceObserver.
 */

#include "driver/types.h"
#include <vector>
#include <cstdio>

/**
 * @brief Observe data including service time.
 * @ingroup logger
 *
 * This is mainly for 2HR-f and its extended algorithms.
 */
class ServiceObserver {

  public:

    /**
     * @brief Record a service time, including a start and an end time.
     */
    class Service {

      public:

        /**
         * @brief Construct with both start and end time.
         */
        Service(int start, int end) : start(start), end(end) {
        }

        /**
         * @brief Start time of the service.
         */
        int start;

        /**
         * @brief End time of the service.
         */
        int end;

    };

    /**
     * @brief Create instance.
     * @return The instance that should be deleted by user.
     */
    static ServiceObserver* create(Driver& driver);

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
     * @brief Called when the sender is starting dispatching packets.
     */
    void log_send_start(int tag, int tick);

    /**
     * @brief Called when the dispatching process is over.
     *
     * This could happen either when f copies are dispatched or the head is
     * timed out.
     */
    void log_send_end(int tag, int tick);

    /**
     * @brief Called when the receiver is starting receiving a packet.
     */
    void log_receive_start(int tag, int tick);

    /**
     * @brief Called when the receiving process is over.
     */
    void log_receive_end(int tag, int tick);

  private:

    typedef std::vector<Service> ServiceTime;
    
    ServiceObserver(Driver& driver, double ratio);

    Driver& driver;
    int generated, dispatched, received, last_dispatch;
    int starting_tick;
    ServiceTime send_time, receive_time;

    

};

#endif 
