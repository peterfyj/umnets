#ifndef LOGGER_PRINTER_H_
#define LOGGER_PRINTER_H_

#include "driver/types.h"
#include "stdio.h"

class Printer {

  public:

    typedef struct {
      bool log_msg;
      bool log_before_simulation;
      bool log_after_simulation;
      bool log_before_loop;
      bool log_after_loop;
      bool log_node_moved;
      bool log_packet_generated;
      bool log_packet_transfered;
    } Switch;

    static Printer* create(Driver& driver);
    static void announce_options(Driver& driver);

    template<typename...T>
    void log(const char* format, T...arg) const {
      if (control.log_msg) {
        printf(format, arg...);
      }
    }

    void before_simulation();

    void after_simulation();

    void before_loop();

    void after_loop();

    void node_moved(Node& node);

    void packet_generated(Node& where, Packet& packet);

    void packet_transfered(Node& from, Node& to, Packet& packet);

  private:

    Printer(Driver& driver, const Switch& control);
    Switch control;
    Driver& driver;

};

#endif
